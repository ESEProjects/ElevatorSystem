# ElevatorSystem
An elevator system that includes CAN bus, internet access via PHP interface, and activity logging via SQL.

The elevator system consists of 5 nodes connected to CAN bus and act as the physical interface and the elevator actuator. There are three nodes dedicated to each floor as call stations, one node as the master controller and car actuator, and the last node acts as the bridge between the CAN network and the internet/LAN network.

A SQL database holds all CAN frames seen on the bus and make them available to a PHP webpage for elevator diagnostic display and remote elevator control.
