enum SlotStatus { available, occupied }

class ParkingSlot {
  final String id;
  final SlotStatus status;

  ParkingSlot({required this.id, required this.status});
}
