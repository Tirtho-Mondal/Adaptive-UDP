
# A Genetic Algorithm Approach to Adaptive Packet Size Control for UDP

---



# Table of Contents

1. [Abstract](#abstract)  
2. [Introduction](#introduction)  
   - Motivation and Problem Context  
   - Aim of the Project  
   - Objectives  
   - Scope and Significance  
3. [Background Study](#background-study)  
   - UDP Characteristics  
   - Congestion Control Mechanisms  
   - Fundamentals of Genetic Algorithm  
4. [System Design and Architecture](#system-design-and-architecture)  
   - Network Topology  
   - Sender Node  
   - Router Nodes  
   - Receiver Node  
5. [Overall Workflow](#overall-workflow)  
6. [Discussion](#discussion)  
7. [Conclusion](#conclusion)  
8. [Appendix A — Main Initialization Parameters](#appendix-a--main-initialization-parameters)  
9. [Appendix B — Core Equations Summary](#appendix-b--core-equations-summary)  

---

# Abstract

UDP (User Datagram Protocol) is known for its speed and efficiency in transmitting real-time data but lacks essential features such as congestion control, reliability, and adaptability, often resulting in packet loss and reduced network performance during congestion.

To overcome these limitations, this study introduces an intelligent adaptive packet size control mechanism based on a Genetic Algorithm (GA). The GA operates at the router level, dynamically adjusting packet size recommendations according to a computed Congestion Index (CI) that reflects the network’s real-time state.

Sender nodes adapt their transmission sizes based on this feedback, forming a self-regulating, congestion-aware UDP framework.

Implemented in the OMNeT++ simulation environment using C++ for module definitions and evolutionary logic, the proposed approach demonstrates that GA-based adaptive control enhances throughput stability, minimizes packet loss, and maintains consistent network performance under varying load conditions.

These findings highlight the effectiveness of bio-inspired learning techniques in improving traditional non-congestion-aware protocols like UDP, bridging the gap between transmission efficiency and adaptive network behavior.

---

# Introduction

## Motivation and Problem Context

With the increasing use of multimedia services, IoT devices, and real-time communication systems, the demand for fast and low-latency network protocols has grown significantly.

UDP is a popular choice for time-sensitive applications like:

- Video streaming
- Online gaming
- VoIP
- Real-time communication

because it offers minimal overhead and quick data transmission.

However, its lack of congestion control makes it vulnerable to instability when network traffic is heavy.

As multiple UDP senders transmit at high speeds, routers can become congested, causing queue overflows and packet loss.

Unlike TCP, which automatically slows down during congestion, UDP continues sending data at a constant rate and packet size, intensifying network congestion.

This limitation highlights the need for an adaptive mechanism that can intelligently regulate UDP’s transmission behavior under varying network conditions without changing its core design.

---

## Aim of the Project

This project aims to design and evaluate a Genetic Algorithm (GA)-based adaptive control system that dynamically adjusts UDP packet sizes according to real-time network congestion feedback.

By applying evolutionary principles, the system continuously evolves optimal transmission parameters, ensuring efficient and stable performance under varying network conditions.

---

## Objectives

- To optimize packet size dynamically based on real-time network conditions.
- To reduce packet loss and congestion in varying network environments.
- To improve overall data transmission efficiency and throughput.
- To simulate and analyze adaptive behavior using the OMNeT++ network simulator.

---

## Scope and Significance

Unlike conventional TCP congestion control, which relies on window scaling and acknowledgments, this GA-based approach focuses purely on packet size adaptation making it lightweight, scalable, and suitable for connectionless environments.

The approach can be generalized for:

- IoT systems
- Sensor networks
- Real-time communication systems

---

# Background Study

## UDP Characteristics

The User Datagram Protocol (UDP) is a connectionless transport layer protocol that enables rapid data transmission with minimal overhead.

It does not establish a connection or guarantee reliable delivery, making it suitable for applications where speed and efficiency are prioritized over reliability.

### UDP Features

| Feature | Description |
|---|---|
| Type | Connectionless and message-oriented |
| Reliability | No acknowledgment or retransmission |
| Speed | High-speed transmission |
| Header Size | 8 bytes |
| Error Checking | Optional checksum |

---

## Congestion Control Mechanisms

Congestion control in UDP using Genetic Algorithm (GA) aims to manage network traffic efficiently since UDP itself lacks built-in congestion control.

The Genetic Algorithm is applied to optimize parameters such as:

- Packet sending rate
- Packet size
- Loss recovery

based on real-time network feedback.

By evolving the best transmission strategies through:

- Selection
- Crossover
- Mutation

GA helps reduce:

- Packet loss
- Delay
- Congestion

while improving throughput.

---

# Fundamentals of Genetic Algorithm

1. **Initialization**  
   A population of random candidate solutions is generated.

2. **Fitness Evaluation**  
   Each solution is evaluated using a fitness function.

3. **Selection**  
   The best-performing solutions are selected as parents.

4. **Crossover**  
   Parents exchange genetic information to create offspring.

5. **Mutation**  
   Random changes maintain diversity.

6. **Replacement**  
   The new generation replaces the old population.

GA’s adaptability and robustness make it ideal for optimizing parameters in dynamic communication systems.

---

# System Design and Architecture

## Network Topology

The simulated network includes:

1. Three SenderNodes transmitting UDP-like packets.
2. Two RouterNodes performing packet forwarding and GA-based adaptation.
3. Two ReceiverNodes collecting packets for performance measurement.

---

<p align="center">
  <img src="assets/Network_UDP.png" width="800"/>
</p>

<div align="center">

**Figure 1: Architecture of GA-based adaptive UDP network**

</div>

---

The goal is to simulate adaptive communication where senders adjust packet size based on congestion feedback and routers evolve routing and transmission strategies using a Genetic Algorithm.

---

# Sender Node

The sender node generates packets at regular intervals and adjusts their size based on feedback from the router.

### Packet Generation

```math
P_k = \text{Packet of size } S_k \text{ bytes sent at time } t_k
```

where

```math
t_k = k \times T_s
```

```math
T_s = 0.2 \text{ seconds}
```

---

<p align="center">
  <img src="assets/Initially sending paket.png" width="900"/>
</p>

<div align="center">

**Figure 2: Initial Stage of UDP Packet Transmission**

</div>

---

Based on the congestion index (CI), the packet size is updated as:

```math
S_{k+1} =
\begin{cases}
\min(1500, S_k + 100), & \text{if } CI < 0.3 \\
S_k, & \text{if } 0.3 \le CI < 0.6 \\
\max(400, S_k - 150), & \text{if } 0.6 \le CI < 0.8 \\
\max(200, S_k - 300), & \text{if } CI \ge 0.8
\end{cases}
```

When congestion is low, packet size increases to use more bandwidth.

At medium congestion, it remains constant.

At high congestion, packet size decreases to reduce network load.

---

# Router Nodes

The Genetic Algorithm (GA) within the Router Node model evolves optimal routing and packet-size strategies by iteratively refining a population of candidate solutions.

---

## 1. Initialization

Each individual is represented as:

```math
I_i = \{ \text{path}_i, \text{packetSize}_i, \text{fitness}_i \}
```

Where:

- `path_i` = routing path index
- `packetSize_i` = packet size
- `fitness_i` = evaluated performance

---

## 2. Fitness Evaluation

The fitness function is defined as:

```math
f = 1 - CI
```

where the congestion index is:

```math
CI = 0.7 \times \frac{Q}{C} + 0.3 \times \frac{D}{D + 1}
```

Where:

| Symbol | Meaning |
|---|---|
| Q | Queue length |
| C | Channel capacity |
| D | Packet drop ratio |

---

<p align="center">
  <img src="assets/Feedback.png" width="900"/>
</p>

<div align="center">

**Figure 3: Feedback Flow to the Sender**

</div>

---

## 3. Selection

Individuals with higher fitness values are more likely to be selected as parents.

This simulates the principle of:

> Survival of the fittest

---

## 4. Crossover (Recombination)

```math
\text{child.path} = \text{parent2.path}
```

```math
\text{child.packetSize} = \text{parent1.packetSize}
```

---

## 5. Mutation

```math
\text{child.path} \leftarrow \text{random}(0, \text{gateSize}-1)
```

```math
\text{child.packetSize} \leftarrow \text{random}(400,1500)
```

Mutation prevents premature convergence and maintains diversity.

---

## 6. Parameter Adjustment (Adaptive Control)

Mutation rate update:

```math
m_{\text{new}} =
\begin{cases}
1.1 \times m, & \text{if improvement} < 0.01 \\
0.95 \times m, & \text{otherwise}
\end{cases}
```

If diversity decreases:

```math
m_{\text{new}} = 1.2m
```

```math
c_{\text{new}} = 0.9c
```

Otherwise:

```math
c_{\text{new}} = 1.1c
```

---

## 7. Replacement

New offspring replace the least-fit individuals in the population.

---

## 8. Termination

The GA continues until:

- Maximum generation count is reached
- Fitness improvement becomes negligible
- Network performance stabilizes

---

<p align="center">
  <img src="assets/diff inin.png" width="900"/>
</p>

<div align="center">

**Figure 4: The updated packet size is sent by the sender**

</div>

---

# Receiver Node

The receiver node receives packets and maintains a count of successfully delivered packets.

```math
N_r = \text{TotalPacketsReceived}
```

---

<p align="center">
  <img src="assets/Screenshot 2025-10-30 000802.png" width="900"/>
</p>

<div align="center">

**Figure 5: The updated packet size is sent to the receiver**

</div>

---

# Overall Workflow

1. Sender transmits packets at regular intervals.
2. Router receives and buffers packets.
3. Router applies Genetic Algorithm optimization.
4. Router computes Congestion Index.
5. Feedback is sent to sender nodes.
6. Sender adjusts packet size.
7. Receiver logs successful packets.

---

# Discussion

The proposed GA mechanism introduced an intelligent decentralized control layer without altering UDP headers or requiring additional control channels.

By learning from congestion feedback, the system achieved:

- Dynamic equilibrium
- Congestion-aware transmission
- Throughput stabilization
- Reduced packet loss

Unlike heuristic control schemes, GA-based adaptation exhibits resilience under fluctuating conditions.

The probabilistic nature of GA allows continuous exploration, preventing stagnation even in steady-state networks.

---

# Conclusion

This work demonstrated that Genetic Algorithms can effectively enable congestion-aware and adaptive packet size control in UDP.

The OMNeT++ simulations showed that GA-based adaptation:

- Reduces packet loss
- Stabilizes throughput
- Maintains fairness among senders

This confirms that evolutionary methods can improve traditional UDP communication with minimal computational overhead.

---

# Future Work

Future improvements may include:

- Reinforcement Learning integration
- Wireless network optimization
- Multi-objective GA models
- IoT deployment
- Energy-efficient routing

---

# Appendix A — Main Initialization Parameters

| Component | Parameter | Initial Value | Description |
|---|---|---|---|
| Simulation | sim-time-limit | 100 s | Total duration |
| Simulation | record-eventlog | true | Enables event logging |
| All Modules | capacity | 100 | Queue capacity |
| SenderNode | packetSize | 800 bytes | Initial packet size |
| SenderNode | sendInterval | 0.2 s | Transmission interval |
| RouterNode | mutationRate | 0.12 | Mutation probability |
| RouterNode | crossoverRate | 0.8 | Crossover probability |
| RouterNode | population size | 10 | Number of individuals |
| sender → router | delay | 1 ms | Link delay |
| sender → router | datarate | 10 Mbps | Link bandwidth |
| router1 → router2 | delay | 2 ms | Intermediate delay |
| router1 → router2 | datarate | 5 Mbps | Intermediate bandwidth |
| router2 → receiver | delay | 2 ms | Receiver delay |
| router2 → receiver | datarate | 8 Mbps | Receiver bandwidth |

---

# Appendix B — Core Equations Summary

| Concept | Equation | Description |
|---|---|---|
| Packet Generation | \( t_k = k \times T_s \) | Packet generation interval |
| Congestion Index | \( CI = 0.7 \times \frac{Q}{C} + 0.3 \times \frac{D}{D+1} \) | Measures congestion |
| Fitness Function | \( f = 1 - CI \) | Measures routing performance |
| Packet Adaptation | Dynamic CI-based update | Adjusts packet size |
| Mutation Update | Adaptive mutation control | Maintains diversity |
| Receiver Metric | \( N_r = TotalPacketsReceived \) | Measures received packets |

---

# Repository Structure

```bash
Adaptive-UDP/
│
├── src/
│   ├── SenderNode.cc
│   ├── RouterNode.cc
│   ├── ReceiverNode.cc
│
├── simulations/
│   ├── omnetpp.ini
│   ├── network.ned
│
├── assets/
│   ├── images
│   ├── diagrams
│
├── README.md
└── references.bib
```

---

# Technologies Used

| Technology | Purpose |
|---|---|
| OMNeT++ | Network Simulation |
| C++ | Core Implementation |
| Genetic Algorithm | Adaptive Optimization |
| UDP Protocol | Communication |
| Queue Management | Congestion Handling |

---

# Author

**Tirtho Mondal**  
Department of Computer Science and Engineering  
Khulna University of Engineering & Technology (KUET)

---
