```mermaid
sequenceDiagram
  participant M01 as M01 (Feeder)
  participant M02 as M02 (Conveyor)
  participant M03 as M03 (Classification)
  participant M04 as M04 (Counter)

  M01->>M01: Material Feeding
  M01->>M02: Material Output and Transfer
  M02->>M03: Material Movement
  M03->>M03: Stop Material with Servo Motor
  M03->>M03: Detect Color with Camera
  M03->>M03: Send Data
  M03->>M04: Operate Servo Motor for Sorting
  M04->>M04: Counting
  M04->>M04: Material Sorting

  note right of M03: Color detection and data transmission
  note right of M04: Sorting and counting process


