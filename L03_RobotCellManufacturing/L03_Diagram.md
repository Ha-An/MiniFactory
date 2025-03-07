# MiniFactory


## L03: Robot Cell Manufacturing

```mermaid
sequenceDiagram
    participant IN/OUT
    participant Robot_Arm
    participant Station_1
    participant Station_2
    participant Station_3

    Robot_Arm->>Station_1: Tranfer
    Station_1-->>Robot_Arm: Process 1 (15s)

    Robot_Arm->>Station_2: Tranfer
    Station_2-->>Robot_Arm: Process 2 (15s)

    Robot_Arm->>Station_3: Tranfer
    Station_3-->>Robot_Arm: Process 3 (17s)

    Robot_Arm->>IN/OUT: Tranfer & Discharge