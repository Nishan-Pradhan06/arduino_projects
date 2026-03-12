import 'package:flutter/material.dart';
import 'package:dio/dio.dart';
import 'dart:async';
import 'dart:math' as math;

class RoverControl extends StatefulWidget {
  const RoverControl({super.key});

  @override
  State<RoverControl> createState() => _RoverControlState();
}

class _RoverControlState extends State<RoverControl>
    with TickerProviderStateMixin {
  final Dio dio = Dio();
  String temperature = "--";
  Timer? commandTimer;
  late AnimationController _pulseController;
  late AnimationController _glowController;
  String? _activeCommand;

  // Color palette
  static const Color bgDark = Color(0xFF0A0D14);
  static const Color bgCard = Color(0xFF111827);
  static const Color accent = Color(0xFFFF4500); // Mars red-orange
  static const Color accentGlow = Color(0x44FF4500);
  static const Color accentDim = Color(0xFF7A2000);
  static const Color textPrimary = Color(0xFFE8E8F0);
  static const Color textSecondary = Color(0xFF8892A4);
  static const Color gridLine = Color(0xFF1C2333);

  @override
  void initState() {
    super.initState();
    _pulseController = AnimationController(
      vsync: this,
      duration: const Duration(seconds: 2),
    )..repeat(reverse: true);

    _glowController = AnimationController(
      vsync: this,
      duration: const Duration(milliseconds: 800),
    )..repeat(reverse: true);

    startTempStream();
  }

  @override
  void dispose() {
    _pulseController.dispose();
    _glowController.dispose();
    commandTimer?.cancel();
    super.dispose();
  }

  void startTempStream() {
    Timer.periodic(const Duration(seconds: 2), (timer) async {
      try {
        var res = await dio.get("http://192.168.4.1/temp");
        setState(() {
          temperature = res.data.toString();
        });
      } catch (e) {
        debugPrint("Temp Error: $e");
      }
    });
  }

  void sendCommand(String cmd) async {
    try {
      await dio.get("http://192.168.4.1/$cmd");
    } catch (e) {
      debugPrint("Command Error: $e");
    }
  }

  void startCommand(String cmd) {
    setState(() => _activeCommand = cmd);
    sendCommand(cmd);
    commandTimer?.cancel();
    commandTimer = Timer.periodic(const Duration(milliseconds: 200), (_) {
      sendCommand(cmd);
    });
  }

  void stopCommand() {
    setState(() => _activeCommand = null);
    commandTimer?.cancel();
    sendCommand("stop");
  }

  Widget _buildDirectionButton({
    required IconData icon,
    required String cmd,
    required String label,
    double size = 64,
  }) {
    final isActive = _activeCommand == cmd;

    return GestureDetector(
      onTapDown: (_) => startCommand(cmd),
      onTapUp: (_) => stopCommand(),
      onTapCancel: stopCommand,
      child: AnimatedBuilder(
        animation: _glowController,
        builder: (context, child) {
          final glowIntensity = isActive
              ? 0.6 + _glowController.value * 0.4
              : 0.0;
          return Container(
            width: size,
            height: size,
            decoration: BoxDecoration(
              shape: BoxShape.circle,
              color: isActive ? accent.withOpacity(0.15) : bgCard,
              border: Border.all(
                color: isActive ? accent : const Color(0xFF2A3448),
                width: isActive ? 2 : 1,
              ),
              boxShadow: isActive
                  ? [
                      BoxShadow(
                        color: accent.withOpacity(glowIntensity * 0.6),
                        blurRadius: 20,
                        spreadRadius: 4,
                      ),
                      BoxShadow(
                        color: accent.withOpacity(glowIntensity * 0.3),
                        blurRadius: 40,
                        spreadRadius: 8,
                      ),
                    ]
                  : [
                      BoxShadow(
                        color: Colors.black.withOpacity(0.4),
                        blurRadius: 8,
                        offset: const Offset(0, 2),
                      ),
                    ],
            ),
            child: Icon(
              icon,
              color: isActive ? accent : textSecondary,
              size: 28,
            ),
          );
        },
      ),
    );
  }

  Widget _buildStopButton() {
    final isActive = _activeCommand == "stop";
    return GestureDetector(
      onTapDown: (_) => startCommand("stop"),
      onTapUp: (_) => stopCommand(),
      onTapCancel: stopCommand,
      child: Container(
        width: 56,
        height: 56,
        decoration: BoxDecoration(
          shape: BoxShape.circle,
          color: isActive ? const Color(0xFF2A1010) : bgCard,
          border: Border.all(
            color: isActive ? Colors.red : const Color(0xFF2A3448),
            width: isActive ? 2 : 1,
          ),
        ),
        child: const Icon(
          Icons.stop_rounded,
          color: Colors.redAccent,
          size: 24,
        ),
      ),
    );
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      backgroundColor: bgDark,
      body: Stack(
        children: [
          // Grid background
          CustomPaint(
            painter: _GridPainter(),
            size: MediaQuery.of(context).size,
          ),

          SafeArea(
            child: Column(
              children: [
                // Divider
                Container(height: 1, color: gridLine),

                const Spacer(),

                Row(
                  children: [
                    Container(
                      padding: const EdgeInsets.all(10),
                      decoration: BoxDecoration(
                        color: accent.withOpacity(0.1),
                        borderRadius: BorderRadius.circular(10),
                        border: Border.all(
                          color: accent.withOpacity(0.3),
                          width: 1,
                        ),
                      ),
                      child: const Icon(
                        Icons.thermostat_rounded,
                        color: accent,
                        size: 22,
                      ),
                    ),
                    const SizedBox(width: 16),
                    Column(
                      crossAxisAlignment: CrossAxisAlignment.start,
                      children: [
                        const Text(
                          "SURFACE TEMP",
                          style: TextStyle(
                            fontFamily: 'monospace',
                            fontSize: 10,
                            letterSpacing: 2.5,
                            color: textSecondary,
                          ),
                        ),
                        const SizedBox(height: 4),
                        Row(
                          crossAxisAlignment: CrossAxisAlignment.baseline,
                          textBaseline: TextBaseline.alphabetic,
                          children: [
                            Text(
                              temperature,
                              style: const TextStyle(
                                fontFamily: 'monospace',
                                fontSize: 36,
                                fontWeight: FontWeight.bold,
                                color: textPrimary,
                                height: 1,
                              ),
                            ),
                            const SizedBox(width: 6),
                            const Text(
                              "°C",
                              style: TextStyle(
                                fontFamily: 'monospace',
                                fontSize: 16,
                                color: textSecondary,
                              ),
                            ),
                          ],
                        ),
                      ],
                    ),
                    const Spacer(),
                    AnimatedBuilder(
                      animation: _pulseController,
                      builder: (context, _) => Icon(
                        Icons.wifi_rounded,
                        color: Colors.greenAccent.withOpacity(
                          0.5 + _pulseController.value * 0.5,
                        ),
                        size: 18,
                      ),
                    ),
                  ],
                ),

                const Spacer(),

                Padding(
                  padding: const EdgeInsets.symmetric(horizontal: 40.0),
                  child: Row(
                    mainAxisAlignment: MainAxisAlignment.spaceBetween,
                    children: [
                      Column(
                        spacing: 20,
                        children: [
                          // Forward
                          _buildDirectionButton(
                            icon: Icons.keyboard_arrow_up_rounded,
                            cmd: "backward",
                            label: "REV",
                            size: 70,
                          ),
                          _buildDirectionButton(
                            icon: Icons.keyboard_arrow_down_rounded,
                            cmd: "forward",
                            label: "FWD",

                            size: 70,
                          ),
                        ],
                      ),
                      Row(
                        mainAxisAlignment: MainAxisAlignment.center,
                        children: [
                          _buildDirectionButton(
                            icon: Icons.keyboard_arrow_left_rounded,
                            cmd: "left",
                            label: "L",
                            size: 70,
                          ),

                          const SizedBox(width: 16),
                          _buildDirectionButton(
                            icon: Icons.keyboard_arrow_right_rounded,
                            cmd: "right",
                            label: "R",
                            size: 70,
                          ),
                        ],
                      ),
                    ],
                  ),
                ),

                // D-pad controller
                const Spacer(),

                // Bottom status bar
                Container(height: 1, color: gridLine),
                Padding(
                  padding: const EdgeInsets.symmetric(
                    horizontal: 24,
                    vertical: 12,
                  ),
                  child: Row(
                    children: [
                      const Text(
                        "192.168.4.1",
                        style: TextStyle(
                          fontFamily: 'monospace',
                          fontSize: 11,
                          color: textSecondary,
                          letterSpacing: 1,
                        ),
                      ),
                      const Spacer(),
                      Text(
                        _activeCommand != null
                            ? _activeCommand!.toUpperCase()
                            : "IDLE",
                        style: TextStyle(
                          fontFamily: 'monospace',
                          fontSize: 11,
                          letterSpacing: 2,
                          color: _activeCommand != null
                              ? accent
                              : textSecondary,
                        ),
                      ),
                    ],
                  ),
                ),
              ],
            ),
          ),
        ],
      ),
    );
  }
}

class _GridPainter extends CustomPainter {
  @override
  void paint(Canvas canvas, Size size) {
    final paint = Paint()
      ..color = const Color(0xFF1A2030)
      ..strokeWidth = 0.5;

    const spacing = 40.0;

    for (double x = 0; x < size.width; x += spacing) {
      canvas.drawLine(Offset(x, 0), Offset(x, size.height), paint);
    }
    for (double y = 0; y < size.height; y += spacing) {
      canvas.drawLine(Offset(0, y), Offset(size.width, y), paint);
    }

    // Radial fade from center — subtle vignette
    final vignette = RadialGradient(
      center: Alignment.center,
      radius: 0.9,
      colors: [Colors.transparent, Colors.black.withOpacity(0.5)],
    ).createShader(Rect.fromLTWH(0, 0, size.width, size.height));

    canvas.drawRect(
      Rect.fromLTWH(0, 0, size.width, size.height),
      Paint()..shader = vignette,
    );
  }

  @override
  bool shouldRepaint(_GridPainter oldDelegate) => false;
}
