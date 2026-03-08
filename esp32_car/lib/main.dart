import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'pages/remote_control_nrfs.dart';
import 'pages/soccer_bot.dart';
import 'widgets/card_widget.dart';

void main() async {
  WidgetsFlutterBinding.ensureInitialized();

  // Lock orientation to landscape
  await SystemChrome.setPreferredOrientations([
    DeviceOrientation.landscapeLeft,
    DeviceOrientation.landscapeRight,
  ]);

  runApp(const CarControllerApp());
}

class CarControllerApp extends StatelessWidget {
  const CarControllerApp({super.key});

  @override
  Widget build(BuildContext context) {
    return const MaterialApp(
      debugShowCheckedModeBanner: false,
      home: HomeScreen(),
    );
  }
}

class HomeScreen extends StatelessWidget {
  const HomeScreen({super.key});

  @override
  Widget build(BuildContext context) {
    final screenWidth = MediaQuery.of(context).size.width;

    return Scaffold(
      body: Padding(
        padding: const EdgeInsets.symmetric(vertical: 40),
        child: SingleChildScrollView(
          scrollDirection: Axis.horizontal,
          physics: const BouncingScrollPhysics(),
          child: ConstrainedBox(
            constraints: BoxConstraints(
              minWidth: screenWidth, // ensures the row fills the screen width
            ),
            child: Center(
              child: Row(
                mainAxisSize: MainAxisSize.min, // important: wrap content
                children: [
                  CarCard(
                    title: 'Remote Control Car',
                    imageUrl:
                        'https://cdn-icons-png.flaticon.com/512/743/743922.png',
                    onTap: () {
                      Navigator.of(context).push(
                        MaterialPageRoute(
                          builder: (context) => RemoteControlNrfs(),
                        ),
                      );
                    },
                  ),
                  SizedBox(width: 20),
                  CarCard(
                    title: 'Soccer Bot',
                    imageUrl:
                        'https://cdn-icons-png.flaticon.com/512/744/744922.png',
                        onTap: () {
                           Navigator.of(context).push(
                        MaterialPageRoute(
                          builder: (context) => CarControlScreen(),
                        ),
                      );
                        },
                  ),
                  SizedBox(width: 20),
                  CarCard(
                    title: 'Settings',
                    imageUrl:
                        'https://cdn-icons-png.flaticon.com/512/745/745922.png',
                  ),
                ],
              ),
            ),
          ),
        ),
      ),
    );
  }
}
