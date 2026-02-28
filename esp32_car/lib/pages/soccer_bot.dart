// import 'dart:convert';
// import 'dart:developer';

// import 'package:flutter/material.dart';
// import 'package:flutter_bluetooth_serial/flutter_bluetooth_serial.dart';

// class SoccerBot extends StatefulWidget {
//   const SoccerBot({super.key});

//   @override
//   State<SoccerBot> createState() => _SoccerBotState();
// }

// class _SoccerBotState extends State<SoccerBot> {
//   List<BluetoothDevice> devicesList = [];
//   BluetoothConnection? connection;
//   bool isConnected = false;
//   BluetoothDevice? selectedDevice;

//   @override
//   void initState() {
//     super.initState();
//     getPairedDevices();
//   }

//   // Get all paired devices
//   void getPairedDevices() async {
//     List<BluetoothDevice> devices = [];
//     try {
//       devices = await FlutterBluetoothSerial.instance.getBondedDevices();
//     } catch (e) {
//       log('Error getting paired devices: $e');
//     }
//     setState(() {
//       devicesList = devices;
//     });
//   }

//   // Connect to selected device
//   void connectToDevice(BluetoothDevice device) async {
//     try {
//       connection = await BluetoothConnection.toAddress(device.address);
//       setState(() {
//         isConnected = true;
//         selectedDevice = device;
//       });
//       log('Connected to ${device.name}');
//     } catch (e) {
//       log('Cannot connect: $e');
//     }
//   }

//   // Send command to Arduino
//   void sendCommand(String command) {
//     if (connection != null && isConnected) {
//       connection!.output.add(utf8.encode(command));
//       log("Sent: $command");
//     }
//   }

//   @override
//   void dispose() {
//     connection?.dispose();
//     super.dispose();
//   }

//   @override
//   Widget build(BuildContext context) {
//     return Scaffold(
//       appBar: AppBar(title: Text("Arduino Car Controller")),
//       body: isConnected ? buildControlButtons() : buildDeviceSelection(),
//     );
//   }

//   // Device selection UI
//   Widget buildDeviceSelection() {
//     return Column(
//       children: [
//         SizedBox(height: 20),
//         Text("Select HC-05 device to connect", style: TextStyle(fontSize: 18)),
//         Expanded(
//           child: ListView.builder(
//             itemCount: devicesList.length,
//             itemBuilder: (context, index) {
//               BluetoothDevice device = devicesList[index];
//               return ListTile(
//                 title: Text(device.name ?? "Unknown"),
//                 subtitle: Text(device.address),
//                 onTap: () => connectToDevice(device),
//               );
//             },
//           ),
//         ),
//         ElevatedButton(
//           onPressed: getPairedDevices,
//           child: Text("Refresh Devices"),
//         ),
//       ],
//     );
//   }

//   // Control buttons UI
//   Widget buildControlButtons() {
//     return Column(
//       mainAxisAlignment: MainAxisAlignment.center,
//       children: [
//         Text(
//           "Connected to: ${selectedDevice?.name}",
//           style: TextStyle(fontSize: 16),
//         ),
//         SizedBox(height: 20),
//         Row(
//           mainAxisAlignment: MainAxisAlignment.center,
//           children: [
//             ElevatedButton(
//               onPressed: () => sendCommand("F"),
//               child: Text("Forward"),
//             ),
//           ],
//         ),
//         Row(
//           mainAxisAlignment: MainAxisAlignment.center,
//           children: [
//             ElevatedButton(
//               onPressed: () => sendCommand("TL"),
//               child: Text("Left"),
//             ),
//             SizedBox(width: 20),
//             ElevatedButton(
//               onPressed: () => sendCommand("TR"),
//               child: Text("Right"),
//             ),
//           ],
//         ),
//         Row(
//           mainAxisAlignment: MainAxisAlignment.center,
//           children: [
//             ElevatedButton(
//               onPressed: () => sendCommand("R"),
//               child: Text("Reverse"),
//             ),
//           ],
//         ),
//         SizedBox(height: 20),
//         ElevatedButton(onPressed: () => sendCommand("n"), child: Text("Stop")),
//         SizedBox(height: 20),
//         ElevatedButton(
//           onPressed: () {
//             connection?.dispose();
//             setState(() {
//               isConnected = false;
//               selectedDevice = null;
//             });
//           },
//           child: Text("Disconnect"),
//         ),
//       ],
//     );
//   }
// }
