import 'package:flutter/material.dart';
import 'features/dashboard/dashbaord_page.dart';

void main() => runApp(const SmartCity());

class SmartCity extends StatelessWidget {
  const SmartCity({super.key});

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      debugShowCheckedModeBanner: false,
      theme: ThemeData(
        colorScheme: ColorScheme.fromSeed(
          seedColor: const Color(0xFF0D47A1),
          brightness: Brightness.dark,
        ),
        useMaterial3: true,
      ),
      home: const DashboardScreen(),
    );
  }
}

