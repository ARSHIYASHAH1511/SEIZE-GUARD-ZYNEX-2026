# Unit Test: Seizure Magnitude Threshold Simulator
import math

def test_magnitude_calculation():
    # Simulated MPU6050 Accel data during a seizure
    test_cases = [
        {"x": 0.5, "y": 0.8, "z": 0.9, "expected_alert": True},
        {"x": 0.1, "y": 0.1, "z": 0.9, "expected_alert": False}, # Normal movement
    ]
    
    SEIZURE_THRESHOLD = 1.2
    
    for i, data in enumerate(test_cases):
        magnitude = math.sqrt((data["x"]**2) + (data["y"]**2) + (data["z"]**2))
        is_seizure = magnitude > SEIZURE_THRESHOLD
        
        print(f"Test {i+1}: Magnitude={magnitude:.2f} | Alert={is_seizure}")
        assert is_seizure == data["expected_alert"], f"Test {i+1} Failed!"

    print("✅ All threshold mathematical tests passed.")

if __name__ == "__main__":
    test_magnitude_calculation()
