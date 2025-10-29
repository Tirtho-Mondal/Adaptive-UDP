package adaptive_udp.simulations;

network AdaptiveUDPNetwork
{
    submodules:
        sender1: SenderNode { @display("p=100,100;i=device/pc"); }
        sender2: SenderNode { @display("p=100,160;i=device/pc"); }
        sender3: SenderNode { @display("p=100,220;i=device/pc"); }

        router1: RouterNode { @display("p=300,160;i=abstract/router"); }
        router2: RouterNode { @display("p=500,160;i=abstract/router"); }

        receiver1: ReceiverNode { @display("p=700,120;i=device/laptop"); }
        receiver2: ReceiverNode { @display("p=700,200;i=device/laptop"); }

    connections allowunconnected:
        sender1.out --> { delay=1ms; datarate=10Mbps; } --> router1.in++;
        sender2.out --> { delay=1ms; datarate=10Mbps; } --> router1.in++;
        sender3.out --> { delay=1ms; datarate=10Mbps; } --> router1.in++;
        router1.out++ --> { delay=2ms; datarate=5Mbps; } --> router2.in++;
        router2.out++ --> { delay=2ms; datarate=8Mbps; } --> receiver1.in;
        router2.out++ --> { delay=2ms; datarate=8Mbps; } --> receiver2.in;
}
simple SenderNode
{
    gates:
        input in;
        output out;
//    @display("i=block/app")
}

simple RouterNode
{
    gates:
        input in[];
        output out[];
//    @display("i=block/router")
}

simple ReceiverNode
{
    gates:
        input in;
//    @display("i=block/server")
}
