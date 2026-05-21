import 'dart:convert';

import 'package:flutter/material.dart';
import 'package:web_socket_channel/web_socket_channel.dart';

// ─────────────────────────────────────────────
// Change this URL to match your setup:
//   Android Emulator  → ws://10.0.2.2:3000
//   Physical device   → ws://192.168.x.x:3000  (your PC's local IP)
//   Web / Desktop     → ws://localhost:3000
// ─────────────────────────────────────────────
const String _kWsUrl = 'ws://10.0.2.2:3000';

class TrafficLightScreen extends StatefulWidget {
  const TrafficLightScreen({super.key});

  @override
  State<TrafficLightScreen> createState() => _TrafficLightScreenState();
}

class _TrafficLightScreenState extends State<TrafficLightScreen> {
  WebSocketChannel? _channel;

  // Direction → color string ("red" | "yellow" | "green")
  String _north = 'red';
  String _south = 'red';
  String _east = 'red';
  String _west = 'red';

  bool _connected = false;

  @override
  void initState() {
    super.initState();
    _connect();
  }

  void _connect() {
    try {
      _channel = WebSocketChannel.connect(Uri.parse(_kWsUrl));
      setState(() => _connected = true);

      _channel!.stream.listen(
        (message) {
          final json = jsonDecode(message as String) as Map<String, dynamic>;
          final type = json['type'] as String?;

          if (type == 'init' || type == 'traffic_update') {
            final data = json['data'] as Map<String, dynamic>;
            setState(() {
              _north = (data['north'] as String? ?? 'red').toLowerCase();
              _south = (data['south'] as String? ?? 'red').toLowerCase();
              _east = (data['east'] as String? ?? 'red').toLowerCase();
              _west = (data['west'] as String? ?? 'red').toLowerCase();
            });
          }
        },
        onError: (_) => setState(() => _connected = false),
        onDone: () => setState(() => _connected = false),
      );
    } catch (_) {
      setState(() => _connected = false);
    }
  }

  @override
  void dispose() {
    _channel?.sink.close();
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      backgroundColor: const Color(0xff06111F),
      appBar: AppBar(
        title: Text('Traffic Light'),
        backgroundColor: Colors.transparent,
        elevation: 0,
      ),
      body: SafeArea(
        child: Stack(
          children: [
            /// ROAD BACKGROUND
            Container(
              width: double.infinity,
              height: double.infinity,
              color: const Color(0xff081522),
            ),

            /// VERTICAL ROAD
            Center(
              child: Container(
                width: 150,
                height: double.infinity,
                color: const Color(0xff101C2C),
              ),
            ),

            /// HORIZONTAL ROAD
            Center(
              child: Container(
                width: double.infinity,
                height: 150,
                color: const Color(0xff101C2C),
              ),
            ),

            /// CENTER DIVIDER — VERTICAL
            Center(
              child: Column(
                children: List.generate(
                  20,
                  (i) => Expanded(
                    child: Container(
                      width: 4,
                      margin: const EdgeInsets.symmetric(vertical: 6),
                      color: Colors.amber,
                    ),
                  ),
                ),
              ),
            ),

            /// CENTER DIVIDER — HORIZONTAL
            Center(
              child: Row(
                children: List.generate(
                  20,
                  (i) => Expanded(
                    child: Container(
                      height: 4,
                      margin: const EdgeInsets.symmetric(horizontal: 6),
                      color: Colors.amber,
                    ),
                  ),
                ),
              ),
            ),

            /// TRAFFIC LIGHTS (N / E / W / S)
            _trafficLight(top: 160, left: 55, color: _north), // North
            _trafficLight(top: 160, right: 55, color: _east), // East
            _trafficLight(bottom: 160, left: 55, color: _west), // West
            _trafficLight(bottom: 160, right: 55, color: _south), // South
            /// CONNECTION STATUS BADGE
            Positioned(top: 12, right: 16, child: _connectionBadge()),
          ],
        ),
      ),
    );
  }

  // ─────────────────────────────────────────
  // WIDGETS
  // ─────────────────────────────────────────

  Widget _connectionBadge() {
    final color = _connected ? Colors.greenAccent : Colors.redAccent;
    final label = _connected ? 'LIVE' : 'OFFLINE';
    return GestureDetector(
      onTap: _connected ? null : _connect,
      child: Container(
        padding: const EdgeInsets.symmetric(horizontal: 10, vertical: 4),
        decoration: BoxDecoration(
          color: Colors.black,
          borderRadius: BorderRadius.circular(20),
          border: Border.all(color: color),
          boxShadow: [
            BoxShadow(color: color.withValues(alpha: 0.5), blurRadius: 8),
          ],
        ),
        child: Row(
          mainAxisSize: MainAxisSize.min,
          children: [
            Container(
              width: 8,
              height: 8,
              decoration: BoxDecoration(color: color, shape: BoxShape.circle),
            ),
            const SizedBox(width: 6),
            Text(
              label,
              style: TextStyle(
                color: color,
                fontSize: 11,
                fontWeight: FontWeight.bold,
                letterSpacing: 1,
              ),
            ),
          ],
        ),
      ),
    );
  }

  Widget _trafficLight({
    double? top,
    double? bottom,
    double? left,
    double? right,
    required String color,
  }) {
    // Determine which bulbs are lit
    final isRed = color == 'red';
    final isYellow = color == 'yellow';
    final isGreen = color == 'green';

    // Glow color for the active bulb
    final activeColor = isRed
        ? Colors.redAccent
        : isYellow
        ? Colors.amberAccent
        : Colors.greenAccent;

    // Countdown label (static placeholder — replace with real timer if available)
    final label = isGreen
        ? 'GO'
        : isYellow
        ? 'WAIT'
        : 'STOP';

    return Positioned(
      top: top,
      bottom: bottom,
      left: left,
      right: right,
      child: Column(
        children: [
          Container(
            width: 28,
            height: 80,
            decoration: BoxDecoration(
              color: const Color(0xff0E1726),
              borderRadius: BorderRadius.circular(14),
              border: Border.all(color: Colors.cyanAccent),
            ),
            child: Column(
              mainAxisAlignment: MainAxisAlignment.spaceEvenly,
              children: [
                _circleLight(isRed ? Colors.redAccent : Colors.black),
                _circleLight(isYellow ? Colors.amberAccent : Colors.black),
                _circleLight(isGreen ? Colors.greenAccent : Colors.black),
              ],
            ),
          ),
          const SizedBox(height: 6),
          _neonTag(label, activeColor),
        ],
      ),
    );
  }

  Widget _circleLight(Color color) {
    return Container(
      width: 14,
      height: 14,
      decoration: BoxDecoration(
        color: color,
        shape: BoxShape.circle,
        boxShadow: [
          BoxShadow(color: color.withValues(alpha: 0.8), blurRadius: 10),
        ],
      ),
    );
  }

  Widget _neonTag(String text, Color color) {
    return Container(
      padding: const EdgeInsets.symmetric(horizontal: 8, vertical: 3),
      decoration: BoxDecoration(
        borderRadius: BorderRadius.circular(6),
        color: Colors.black,
        border: Border.all(color: color),
        boxShadow: [
          BoxShadow(color: color.withValues(alpha: 0.4), blurRadius: 6),
        ],
      ),
      child: Text(
        text,
        style: TextStyle(
          color: color,
          fontSize: 10,
          fontWeight: FontWeight.bold,
        ),
      ),
    );
  }
}
