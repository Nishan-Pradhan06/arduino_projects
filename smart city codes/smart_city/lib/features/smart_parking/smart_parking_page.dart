// // ─────────────────────────────────────────────────────────────────────────────
// // DATA MODEL
// // ─────────────────────────────────────────────────────────────────────────────

// import 'package:flutter/material.dart';

// enum SlotStatus { available, occupied }

// class ParkingSlot {
//   final String id;
//   final SlotStatus status;

//   const ParkingSlot({required this.id, required this.status});
// }

// // ─────────────────────────────────────────────────────────────────────────────
// // STATIC DATA  — replace with your real API data later
// // ─────────────────────────────────────────────────────────────────────────────

// final List<ParkingSlot> staticSlots = [
//   ParkingSlot(id: 'F2-23', status: SlotStatus.occupied),
//   ParkingSlot(id: 'F2-24', status: SlotStatus.available),
//   ParkingSlot(id: 'F2-25', status: SlotStatus.available),
//   ParkingSlot(id: 'F2-20', status: SlotStatus.occupied),
//   ParkingSlot(id: 'F2-31', status: SlotStatus.available),
//   ParkingSlot(id: 'F2-32', status: SlotStatus.available),
// ];



// class ParkingLotScreen extends StatelessWidget {
//   const ParkingLotScreen({super.key});

//   int get _freeCount =>
//       staticSlots.where((s) => s.status == SlotStatus.available).length;

//   /// Split flat list into 2-column rows.
//   List<List<ParkingSlot?>> get _rows {
//     final rows = <List<ParkingSlot?>>[];
//     for (int i = 0; i < staticSlots.length; i += 2) {
//       rows.add([
//         staticSlots[i],
//         i + 1 < staticSlots.length ? staticSlots[i + 1] : null,
//       ]);
//     }
//     return rows;
//   }

//   @override
//   Widget build(BuildContext context) {
//     return Scaffold(
//       appBar: AppBar(
//         title: const Text('Smart Parking Monitor'),
//         backgroundColor: const Color(0xFF112236),
//         elevation: 0,
//       ),
//       backgroundColor: const Color(0xFF0D1B2A),
//       body: SafeArea(
//         child: Column(children: [Expanded(child: _buildParkingGrid())]),
//       ),
//     );
//   }

//   // ── Parking Grid ───────────────────────────────────────────────────────────

//   Widget _buildParkingGrid() {
//     return Stack(
//       children: [
//         // Watermark
//         Center(
//           child: RotatedBox(
//             quarterTurns: -1,
//             child: Text(
//               '$_freeCount SLOTS FREE',
//               style: TextStyle(
//                 color: Colors.white.withOpacity(0.04),
//                 fontSize: 24,
//                 fontWeight: FontWeight.w900,
//                 letterSpacing: 8,
//               ),
//             ),
//           ),
//         ),
//         // Grid rows
//         Padding(
//           padding: const EdgeInsets.symmetric(horizontal: 12, vertical: 16),
//           child: Column(
//             mainAxisAlignment: MainAxisAlignment.start,
//             children: _rows.map(_buildRow).toList(),
//           ),
//         ),
//       ],
//     );
//   }

//   Widget _buildRow(List<ParkingSlot?> row) {
//     return Padding(
//       padding: const EdgeInsets.symmetric(vertical: 8),
//       child: Row(
//         children: [
//           Expanded(
//             child: row[0] != null ? _buildSlot(row[0]!) : const SizedBox(),
//           ),
//           const SizedBox(width: 44), // aisle gap
//           Expanded(
//             child: row[1] != null ? _buildSlot(row[1]!) : const SizedBox(),
//           ),
//         ],
//       ),
//     );
//   }

//   Widget _buildSlot(ParkingSlot slot) {
//     final isOccupied = slot.status == SlotStatus.occupied;

//     return Container(
//       height: 90,
//       decoration: BoxDecoration(
//         color: isOccupied
//             ? const Color(0xFF1A3A5C).withOpacity(0.7)
//             : const Color(0xFF0B2619).withOpacity(0.8),
//         borderRadius: BorderRadius.circular(10),
//         border: Border.all(
//           color: isOccupied ? const Color(0xFF2D6A9F) : const Color(0xFF1E5C3A),
//           width: 1.5,
//         ),
//         boxShadow: isOccupied
//             ? null
//             : [
//                 BoxShadow(
//                   color: const Color(0xFF2ECC71).withOpacity(0.07),
//                   blurRadius: 8,
//                 ),
//               ],
//       ),
//       child: Stack(
//         children: [
//           // Corner dashes
//           ..._cornerDashes(isOccupied),
//           // Center content
//           Center(
//             child: Column(
//               mainAxisAlignment: MainAxisAlignment.center,
//               children: [
//                 Icon(
//                   isOccupied
//                       ? Icons.directions_car_filled_rounded
//                       : Icons.crop_free_rounded,
//                   color: isOccupied
//                       ? const Color(0xFF5B9BD5)
//                       : const Color(0xFF4ECDC4).withOpacity(0.3),
//                   size: isOccupied ? 34 : 26,
//                 ),
//                 const SizedBox(height: 5),
//                 Text(
//                   slot.id,
//                   style: TextStyle(
//                     color: isOccupied
//                         ? Colors.white54
//                         : const Color(0xFF4ECDC4).withOpacity(0.6),
//                     fontSize: 11,
//                     fontWeight: FontWeight.w600,
//                     letterSpacing: 0.5,
//                   ),
//                 ),
//               ],
//             ),
//           ),
//         ],
//       ),
//     );
//   }

//   List<Widget> _cornerDashes(bool occupied) {
//     final color = occupied
//         ? const Color(0xFF2D6A9F).withOpacity(0.4)
//         : const Color(0xFF1E5C3A).withOpacity(0.4);
//     const t = 1.5;
//     const l = 10.0;
//     const p = 6.0;
//     return [
//       Positioned(
//         top: p,
//         left: p,
//         child: Container(width: l, height: t, color: color),
//       ),
//       Positioned(
//         top: p,
//         left: p,
//         child: Container(width: t, height: l, color: color),
//       ),
//       Positioned(
//         top: p,
//         right: p,
//         child: Container(width: l, height: t, color: color),
//       ),
//       Positioned(
//         top: p,
//         right: p,
//         child: Container(width: t, height: l, color: color),
//       ),
//       Positioned(
//         bottom: p,
//         left: p,
//         child: Container(width: l, height: t, color: color),
//       ),
//       Positioned(
//         bottom: p,
//         left: p,
//         child: Container(width: t, height: l, color: color),
//       ),
//       Positioned(
//         bottom: p,
//         right: p,
//         child: Container(width: l, height: t, color: color),
//       ),
//       Positioned(
//         bottom: p,
//         right: p,
//         child: Container(width: t, height: l, color: color),
//       ),
//     ];
//   }
// }
import 'dart:convert';

import 'package:flutter/material.dart';
import 'package:http/http.dart' as http;

enum SlotStatus { available, occupied }

class ParkingSlot {
  final String id;
  final SlotStatus status;

  ParkingSlot({required this.id, required this.status});
}

class ParkingLotScreen extends StatefulWidget {
  const ParkingLotScreen({super.key});

  @override
  State<ParkingLotScreen> createState() => _ParkingLotScreenState();
}

class _ParkingLotScreenState extends State<ParkingLotScreen> {
  List<ParkingSlot> slots = [];

  @override
  void initState() {
    super.initState();

    fetchParkingData();

    Future.delayed(const Duration(seconds: 1), () => fetchParkingData());
  }

  Future<void> fetchParkingData() async {
    try {
      final response = await http.get(
        Uri.parse("http://192.168.1.86/parking"),
      );

      final data = jsonDecode(response.body);

      setState(() {
        slots = [
          ParkingSlot(
            id: 'F2-23',
            status: data['slot1'] == 1
                ? SlotStatus.occupied
                : SlotStatus.available,
          ),

          ParkingSlot(
            id: 'F2-24',
            status: data['slot2'] == 1
                ? SlotStatus.occupied
                : SlotStatus.available,
          ),

          ParkingSlot(
            id: 'F2-25',
            status: data['slot3'] == 1
                ? SlotStatus.occupied
                : SlotStatus.available,
          ),

          ParkingSlot(
            id: 'F2-26',
            status: data['slot4'] == 1
                ? SlotStatus.occupied
                : SlotStatus.available,
          ),
        ];
      });
    } catch (e) {
      debugPrint(e.toString());
    }
  }

  int get freeCount =>
      slots.where((s) => s.status == SlotStatus.available).length;

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(title: const Text('Smart Parking')),

      body: GridView.builder(
        padding: const EdgeInsets.all(16),

        itemCount: slots.length,

        gridDelegate: const SliverGridDelegateWithFixedCrossAxisCount(
          crossAxisCount: 2,
          crossAxisSpacing: 12,
          mainAxisSpacing: 12,
        ),

        itemBuilder: (context, index) {
          final slot = slots[index];

          final occupied = slot.status == SlotStatus.occupied;

          return Container(
            decoration: BoxDecoration(
              color: occupied ? Colors.red.shade300 : Colors.green.shade300,

              borderRadius: BorderRadius.circular(16),
            ),

            child: Center(
              child: Column(
                mainAxisAlignment: MainAxisAlignment.center,

                children: [
                  Icon(
                    occupied ? Icons.car_crash : Icons.local_parking,
                    size: 40,
                  ),

                  const SizedBox(height: 10),

                  Text(
                    slot.id,
                    style: const TextStyle(
                      fontSize: 18,
                      fontWeight: FontWeight.bold,
                    ),
                  ),
                ],
              ),
            ),
          );
        },
      ),

      bottomNavigationBar: Container(
        height: 60,

        alignment: Alignment.center,

        child: Text(
          "FREE SLOTS: $freeCount",
          style: const TextStyle(fontSize: 22, fontWeight: FontWeight.bold),
        ),
      ),
    );
  }
}
