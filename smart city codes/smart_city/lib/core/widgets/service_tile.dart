// ── Service tile ──
import 'package:flutter/material.dart';

class ServiceTile extends StatelessWidget {
  final IconData icon;
  final String title;
  final String subtitle;
  final Color statusColor;

  final void Function()? onTap;

  const ServiceTile({
    super.key,
    required this.icon,
    required this.title,
    required this.subtitle,
    required this.statusColor,

    this.onTap,
  });

  @override
  Widget build(BuildContext context) {
    return GestureDetector(
      onTap: onTap,
      child: Container(
        margin: const EdgeInsets.fromLTRB(16, 0, 16, 10),
        decoration: BoxDecoration(
          color: const Color(0xFF0D1B2A),
          borderRadius: BorderRadius.circular(14),
          border: Border.all(color: Colors.white.withValues(alpha: 0.07)),
        ),
        child: ListTile(
          contentPadding: const EdgeInsets.symmetric(
            horizontal: 16,
            vertical: 4,
          ),
          leading: Container(
            padding: const EdgeInsets.all(10),
            decoration: BoxDecoration(
              color: statusColor.withValues(alpha: 0.12),
              borderRadius: BorderRadius.circular(10),
            ),
            child: Icon(icon, color: statusColor, size: 22),
          ),
          title: Text(
            title,
            style: const TextStyle(
              color: Colors.white,
              fontWeight: FontWeight.w600,
              fontSize: 14,
            ),
          ),
          subtitle: Text(
            subtitle,
            style: const TextStyle(color: Colors.white54, fontSize: 12),
          ),
        ),
      ),
    );
  }
}
