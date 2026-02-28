import 'package:flutter/material.dart';
import 'package:dio/dio.dart';

void main() {
  runApp(const IoTControllerApp());
}

class IoTControllerApp extends StatelessWidget {
  const IoTControllerApp({super.key});

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Long Range Controller',
      debugShowCheckedModeBanner: false,
      theme: ThemeData(
        useMaterial3: true,
        brightness: Brightness.dark,
        scaffoldBackgroundColor: const Color(0xFF0F172A),
        colorScheme: ColorScheme.dark(
          primary: const Color(0xFF10B981),
          background: const Color(0xFF0F172A),
          surface: const Color(0xFF1E293B),
        ),
        fontFamily: 'Roboto',
      ),
      home: const ControllerScreen(),
    );
  }
}

class ControllerScreen extends StatefulWidget {
  const ControllerScreen({super.key});

  @override
  State<ControllerScreen> createState() => _ControllerScreenState();
}

class _ControllerScreenState extends State<ControllerScreen>
    with SingleTickerProviderStateMixin {
  bool isPowerOn = false;
  bool isConnected = true;
  bool isLoading = false;
  late AnimationController _animationController;
  late Animation<double> _scaleAnimation;
  final Dio _dio = Dio();

  @override
  void initState() {
    super.initState();
    _animationController = AnimationController(
      duration: const Duration(milliseconds: 200),
      vsync: this,
    );
    _scaleAnimation = Tween<double>(begin: 1.0, end: 0.95).animate(
      CurvedAnimation(parent: _animationController, curve: Curves.easeInOut),
    );
  }

  @override
  void dispose() {
    _animationController.dispose();
    super.dispose();
  }

  Future<void> turnOn() async {
    setState(() => isLoading = true);
    try {
      // Replace with your actual endpoint
      await _dio.post('http://192.168.4.1/on');
      setState(() {
        isPowerOn = true;
        isConnected = true;
      });
    } catch (e) {
      setState(() => isConnected = false);
      _showErrorSnackbar('Failed to turn ON');
    } finally {
      setState(() => isLoading = false);
    }
  }

  Future<void> turnOff() async {
    setState(() => isLoading = true);
    try {
      // Replace with your actual endpoint
      await _dio.post('http://192.168.4.1/off');
      setState(() {
        isPowerOn = false;
        isConnected = true;
      });
    } catch (e) {
      setState(() => isConnected = false);
      _showErrorSnackbar('Failed to turn OFF');
    } finally {
      setState(() => isLoading = false);
    }
  }

  void _showErrorSnackbar(String message) {
    ScaffoldMessenger.of(context).showSnackBar(
      SnackBar(
        content: Text(message),
        backgroundColor: Colors.red.shade700,
        behavior: SnackBarBehavior.floating,
        shape: RoundedRectangleBorder(borderRadius: BorderRadius.circular(10)),
      ),
    );
  }

  void _handlePowerToggle() {
    _animationController.forward().then((_) {
      _animationController.reverse();
    });

    if (isPowerOn) {
      turnOff();
    } else {
      turnOn();
    }
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        backgroundColor: Colors.transparent,
        elevation: 0,
        centerTitle: true,
        title: Column(
          children: [
            const Text(
              'Long Range Controller',
              style: TextStyle(
                fontSize: 22,
                fontWeight: FontWeight.w600,
                letterSpacing: 0.5,
              ),
            ),
            const SizedBox(height: 4),
            Text(
              'NRF24L01 Remote Control',
              style: TextStyle(
                fontSize: 12,
                fontWeight: FontWeight.w400,
                color: Colors.grey.shade500,
                letterSpacing: 0.3,
              ),
            ),
          ],
        ),
      ),
      body: Center(
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children: [
            // Power Button
            ScaleTransition(
              scale: _scaleAnimation,
              child: GestureDetector(
                onTap: isLoading ? null : _handlePowerToggle,
                child: AnimatedContainer(
                  duration: const Duration(milliseconds: 300),
                  width: 200,
                  height: 200,
                  decoration: BoxDecoration(
                    shape: BoxShape.circle,
                    color: isPowerOn
                        ? const Color(0xFF1E293B)
                        : const Color(0xFF1E293B),
                    boxShadow: [
                      BoxShadow(
                        color: isPowerOn
                            ? const Color(0xFF10B981).withOpacity(0.5)
                            : const Color(0xFFEF4444).withOpacity(0.3),
                        blurRadius: 40,
                        spreadRadius: isPowerOn ? 8 : 4,
                      ),
                      BoxShadow(
                        color: Colors.black.withOpacity(0.3),
                        blurRadius: 20,
                        offset: const Offset(0, 10),
                      ),
                    ],
                  ),
                  child: Stack(
                    alignment: Alignment.center,
                    children: [
                      // Inner circle
                      AnimatedContainer(
                        duration: const Duration(milliseconds: 300),
                        width: 160,
                        height: 160,
                        decoration: BoxDecoration(
                          shape: BoxShape.circle,
                          gradient: RadialGradient(
                            colors: isPowerOn
                                ? [
                                    const Color(0xFF10B981).withOpacity(0.3),
                                    const Color(0xFF1E293B),
                                  ]
                                : [
                                    const Color(0xFFEF4444).withOpacity(0.2),
                                    const Color(0xFF1E293B),
                                  ],
                          ),
                        ),
                      ),
                      // Power Icon
                      if (isLoading)
                        const CircularProgressIndicator(
                          color: Colors.white,
                          strokeWidth: 3,
                        )
                      else
                        Icon(
                          Icons.power_settings_new_rounded,
                          size: 80,
                          color: isPowerOn
                              ? const Color(0xFF10B981)
                              : const Color(0xFF64748B),
                        ),
                    ],
                  ),
                ),
              ),
            ),

            const SizedBox(height: 60),

            // Status Text
            AnimatedOpacity(
              duration: const Duration(milliseconds: 300),
              opacity: 1.0,
              child: Column(
                children: [
                  Text(
                    isPowerOn ? 'POWER ON' : 'POWER OFF',
                    style: TextStyle(
                      fontSize: 24,
                      fontWeight: FontWeight.w700,
                      letterSpacing: 2,
                      color: isPowerOn
                          ? const Color(0xFF10B981)
                          : const Color(0xFF64748B),
                    ),
                  ),
                  const SizedBox(height: 12),
                  Row(
                    mainAxisAlignment: MainAxisAlignment.center,
                    children: [
                      Container(
                        width: 8,
                        height: 8,
                        decoration: BoxDecoration(
                          shape: BoxShape.circle,
                          color: isConnected
                              ? const Color(0xFF10B981)
                              : const Color(0xFFEF4444),
                          boxShadow: [
                            BoxShadow(
                              color: isConnected
                                  ? const Color(0xFF10B981).withOpacity(0.5)
                                  : const Color(0xFFEF4444).withOpacity(0.5),
                              blurRadius: 8,
                              spreadRadius: 2,
                            ),
                          ],
                        ),
                      ),
                      const SizedBox(width: 8),
                      Text(
                        isConnected ? 'CONNECTED' : 'DISCONNECTED',
                        style: TextStyle(
                          fontSize: 14,
                          fontWeight: FontWeight.w500,
                          letterSpacing: 1.5,
                          color: isConnected
                              ? const Color(0xFF10B981)
                              : const Color(0xFFEF4444),
                        ),
                      ),
                    ],
                  ),
                ],
              ),
            ),
          ],
        ),
      ),
    );
  }
}
