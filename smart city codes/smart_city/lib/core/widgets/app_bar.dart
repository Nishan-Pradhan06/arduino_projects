// ── App bar title widget ──
import 'package:flutter/material.dart';

class AppBarTitle extends StatelessWidget {
  const AppBarTitle({super.key});

  @override
  Widget build(BuildContext context) {
    return Row(
      mainAxisSize: MainAxisSize.min,
      children: [
        Container(
          padding: const EdgeInsets.all(4),
          decoration: BoxDecoration(
            color: const Color(0xFF29B6F6),
            borderRadius: BorderRadius.circular(6),
          ),
          child: const Icon(Icons.location_city, size: 16, color: Colors.white),
        ),
        const SizedBox(width: 8),
        const Text(
          'Smart Itahari',
          style: TextStyle(
            color: Colors.white,
            fontSize: 18,
            fontWeight: FontWeight.w700,
            letterSpacing: 0.5,
          ),
        ),
      ],
    );
  }
}
