import 'package:flutter/material.dart';
import 'package:smart_city/features/camera/camera.dart';
import 'package:smart_city/features/smart_parking/smart_parking_page.dart';

import '../../core/widgets/app_bar.dart';
import '../../core/widgets/service_tile.dart';
import '../../core/widgets/stats_widget.dart';
import '../traffic_light/traffic_light.dart';

class DashboardScreen extends StatelessWidget {
  const DashboardScreen({super.key});

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      backgroundColor: const Color(0xFF0A0E1A),
      body: CustomScrollView(
        slivers: [
          // ── Custom SliverAppBar with background image ──
          SliverAppBar(
            expandedHeight: 260,
            pinned: true,
            stretch: true,
            backgroundColor: const Color(0xFF0D1B2A),
            flexibleSpace: FlexibleSpaceBar(
              titlePadding: const EdgeInsets.symmetric(
                horizontal: 16,
                vertical: 12,
              ),
              title: const AppBarTitle(),
              background: Stack(
                fit: StackFit.expand,
                children: [
                  // Background image
                  Image.asset(
                    'assets/images.jpg',
                    fit: BoxFit.cover,
                    errorBuilder: (_, _, _) => Container(
                      color: const Color(0xFF0D1B2A),
                      child: const Icon(
                        Icons.location_city,
                        size: 80,
                        color: Colors.white24,
                      ),
                    ),
                  ),
                  // Dark gradient overlay so text is readable
                  Container(
                    decoration: const BoxDecoration(
                      gradient: LinearGradient(
                        begin: Alignment.topCenter,
                        end: Alignment.bottomCenter,
                        colors: [Color(0x33000000), Color(0xCC000F1F)],
                      ),
                    ),
                  ),
                ],
              ),
            ),
          ),

          // ── Stats row ──
          SliverToBoxAdapter(
            child: Padding(
              padding: const EdgeInsets.fromLTRB(16, 20, 16, 8),
              child: Row(
                children: const [
                  Expanded(
                    child: StatCard(
                      icon: Icons.people_alt_outlined,
                      label: 'Population',
                      value: '3.2M',
                      color: Color(0xFF29B6F6),
                    ),
                  ),
                  SizedBox(width: 10),
                  Expanded(
                    child: StatCard(
                      icon: Icons.bolt,
                      label: 'Energy Use',
                      value: '84%',
                      color: Color(0xFFFFCA28),
                    ),
                  ),
                  SizedBox(width: 10),
                  Expanded(
                    child: StatCard(
                      icon: Icons.air,
                      label: 'Air Quality',
                      value: 'Good',
                      color: Color(0xFF66BB6A),
                    ),
                  ),
                ],
              ),
            ),
          ),

          // ── Section label ──
          const SliverToBoxAdapter(
            child: Padding(
              padding: EdgeInsets.fromLTRB(16, 16, 16, 8),
              child: Text(
                'CITY SERVICES',
                style: TextStyle(
                  color: Color(0xFF90CAF9),
                  fontSize: 12,
                  fontWeight: FontWeight.w700,
                  letterSpacing: 2,
                ),
              ),
            ),
          ),

          // ── Service tiles ──
          SliverList(
            delegate: SliverChildListDelegate([
              ServiceTile(
                icon: Icons.local_parking,
                title: 'Smart Parking',
                subtitle: 'All zones operational',
                statusColor: const Color(0xFF66BB6A),

                onTap: () => Navigator.of(context).push(
                  MaterialPageRoute(builder: (context) => ParkingLotScreen()),
                ),
              ),

              ServiceTile(
                icon: Icons.local_hospital_outlined,
                title: 'Emergency Services',
                subtitle: '6 units on standby',
                statusColor: const Color(0xFF66BB6A),
              ),

              ServiceTile(
                icon: Icons.wb_sunny_outlined,
                title: 'Camera Surveillance',
                subtitle: '4,821 / 5,000 active',
                statusColor: const Color(0xFF29B6F6),
                onTap: () {
                  Navigator.of(context).push(
                    MaterialPageRoute(builder: (context) => CameraScreen()),
                  );
                },
              ),
              ServiceTile(
                icon: Icons.wb_sunny_outlined,
                title: 'Traffic Light',
                subtitle: '123',
                statusColor: const Color(0xFF29B6F6),
                onTap: () {
                  Navigator.of(context).push(
                    MaterialPageRoute(
                      builder: (context) => TrafficLightScreen(),
                    ),
                  );
                },
              ),
              const SizedBox(height: 24),
            ]),
          ),
        ],
      ),
    );
  }
}
