// import 'dart:async';
// import 'dart:io';
// import 'package:flutter/material.dart';

// class RemoteControlNrfs extends StatefulWidget {
//   const RemoteControlNrfs({super.key});

//   @override
//   State<RemoteControlNrfs> createState() => _RemoteControlNrfsState();
// }

// class _RemoteControlNrfsState extends State<RemoteControlNrfs> {
//   RawDatagramSocket? socket;

//   final String espIp = "192.168.4.1";
//   final int espPort = 4210;

//   Timer? _timer;

//   @override
//   void initState() {
//     super.initState();
//     initSocket();
//   }

//   Future<void> initSocket() async {
//     socket = await RawDatagramSocket.bind(InternetAddress.anyIPv4, 0);
//   }

//   void sendCommand(String command) {
//     if (socket == null) return;

//     socket!.send(command.codeUnits, InternetAddress(espIp), espPort);
//   }

//   void startSending(String command) {
//     sendCommand(command); // send immediately

//     _timer?.cancel();
//     _timer = Timer.periodic(
//       const Duration(milliseconds: 35), // ULTRA FAST
//       (_) => sendCommand(command),
//     );
//   }

//   void stopSending() {
//     _timer?.cancel();
//     sendCommand('S'); // stop command
//   }

//   Widget controlButton(IconData icon, String command) {
//     return GestureDetector(
//       onTapDown: (_) => startSending(command),
//       onTapUp: (_) => stopSending(),
//       onTapCancel: () => stopSending(),
//       child: Column(
//         children: [
//           Container(),
          
//           Container(
//             width: 120,
//             height: 120,
//             margin: const EdgeInsets.all(12),
//             decoration: const BoxDecoration(
//               color: Colors.blueGrey,
//               shape: BoxShape.circle,
//             ),
//             child: Icon(icon, size: 70, color: Colors.white),
//           ),
//         ],
//       ),
//     );
//   }

//   @override
//   void dispose() {
//     _timer?.cancel();
//     socket?.close();
//     super.dispose();
//   }

//   @override
//   Widget build(BuildContext context) {
//     return Scaffold(
//       body: Center(
//         child: Row(
//           mainAxisAlignment: MainAxisAlignment.spaceAround,
//           children: [
//             // Forward / Back
//             Column(
//               mainAxisSize: MainAxisSize.min,
//               children: [
//                 controlButton(Icons.arrow_upward, 'F'),
//                 const SizedBox(height: 30),
//                 controlButton(Icons.arrow_downward, 'B'),
//               ],
//             ),
//             const SizedBox(width: 40),
//             // Left / Right
//             Row(
//               mainAxisSize: MainAxisSize.min,
//               children: [
//                 controlButton(Icons.arrow_back, 'L'),
//                 const SizedBox(width: 30),
//                 controlButton(Icons.arrow_forward, 'R'),
//               ],
//             ),
//           ],
//         ),
//       ),
//     );
//   }
// }

import 'dart:async';

import 'package:flutter/material.dart';
import 'package:http/http.dart' as http;

class RemoteControlNrfs extends StatefulWidget {
  const RemoteControlNrfs({super.key});

  @override
  State<RemoteControlNrfs> createState() => _RemoteControlNrfsState();
}

class _RemoteControlNrfsState extends State<RemoteControlNrfs> {
  final String baseUrl = 'http://192.168.4.1';
  Timer? _timer;

  Future<void> sendCommand(String command) async {
    try {
      await http
          .get(Uri.parse('$baseUrl/$command'))
          .timeout(const Duration(milliseconds: 300));
    } catch (_) {}
  }

  void startSending(String command) {
    sendCommand(command);
    _timer?.cancel();
    _timer = Timer.periodic(const Duration(milliseconds: 150), (timer) {
      sendCommand(command);
    });
  }

  void stopSending() {
    _timer?.cancel();
    sendCommand('stop');
  }

  Widget controlButton(IconData icon, String command) {
    return GestureDetector(
      onTapDown: (details) => startSending(command),
      onTapUp: (details) => stopSending(),
      onTapCancel: () => stopSending(),
      child: Container(
        width: 120,
        height: 120,
        margin: const EdgeInsets.all(10),
        decoration: const BoxDecoration(
          color: Colors.blueGrey,
          shape: BoxShape.circle,
        ),
        child: Icon(icon, size: 70, color: Colors.white),
      ),
    );
  }

  @override
  void dispose() {
    _timer?.cancel();
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      body: Center(
        child: Row(
          mainAxisAlignment: MainAxisAlignment.spaceAround,
          children: [
            Column(
              mainAxisSize: MainAxisSize.min,
              children: [
                controlButton(Icons.arrow_upward, 'forward'),
                const SizedBox(height: 20),
                controlButton(Icons.arrow_downward, 'back'),
              ],
            ),
            const SizedBox(width: 40),
            Row(
              mainAxisSize: MainAxisSize.min,
              children: [
                controlButton(Icons.arrow_back, 'right'),
                controlButton(Icons.arrow_forward, 'left'),
              ],
            ),
          ],
        ),
      ),
    );
  }
}
