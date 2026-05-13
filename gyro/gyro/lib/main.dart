import 'package:flutter/material.dart';
import 'package:sensors_plus/sensors_plus.dart';
import 'package:http/http.dart' as http;

void main() {
  runApp(MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({super.key});

  @override
  Widget build(BuildContext context) {
    return MaterialApp(home: GyroControl());
  }
}

class GyroControl extends StatefulWidget {
  const GyroControl({super.key});

  @override
  _GyroControlState createState() => _GyroControlState();
}

class _GyroControlState extends State<GyroControl> {
  double gyroX = 0, gyroY = 0;
  double accelX = 0, accelY = 0;

  double finalX = 0, finalY = 0;  

  @override
  void initState() {
    super.initState();

    // 📌 Gyroscope
    gyroscopeEvents.listen((event) {
      gyroX = event.x;
      gyroY = event.y;
    });

    // 📌 Accelerometer
    accelerometerEvents.listen((event) {
      accelX = event.x;
      accelY = event.y;

      // 🔥 Combine both (simple filter)
      finalX = (accelX * 0.7) + (gyroX * 0.3);
      finalY = (accelY * 0.7) + (gyroY * 0.3);

      setState(() {});

      sendData(finalX, finalY);
    });
  }

  // Function to send data to ESP32
  void sendData(double x, double y) async {
    String url = "http://192.168.4.1/update?x=$x&y=$y";

    try {
      await http.get(Uri.parse(url));
      await Future.delayed(Duration(milliseconds: 120)); // prevent spam
    } catch (e) {
      print("Error sending data");
    }
  }

  // 🔥 Manual test function
  void testMotorForward() {
    // send a fixed forward command
    sendData(0, 5); // y > 0.5 → forward
  }

  void testMotorBackward() {
    // send a fixed backward command
    sendData(0, -5); // y < -0.5 → backward
  }

  void stopMotor() {
    sendData(0, 0); // stop
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(title: Text("Gyro + Accelerometer Control")),
      body: Center(
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children: [
            Text(
              "Final X: ${finalX.toStringAsFixed(2)}\n"
              "Final Y: ${finalY.toStringAsFixed(2)}",
              style: TextStyle(fontSize: 22),
              textAlign: TextAlign.center,
            ),
            SizedBox(height: 40),

            // 🔘 Buttons for testing
            ElevatedButton(
              onPressed: testMotorForward,
              child: Text("Test Forward"),  
            ),
            SizedBox(height: 10),
            ElevatedButton(
              onPressed: testMotorBackward,
              child: Text("Test Backward"),
            ),
            SizedBox(height: 10),
            ElevatedButton(
              onPressed: stopMotor,
              child: Text("Stop Motor"),
              style: ElevatedButton.styleFrom(backgroundColor: Colors.red),
            ),
          ],
        ),
      ),
      );
  }
}
