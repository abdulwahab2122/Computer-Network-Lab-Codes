#This program will create 4 nodes and send data via UDP/TCP from one node to another over a duplex-link
set ns [new Simulator]

#creating Two color classes for ns object to distinguish the traffic coming from various sources
$ns color 0 Red
$ns color 1 Black

#Creating nam file
set nf [open out.nam w]
$ns namtrace-all $nf

#Finish procedure to open file using nam to visulaize our network
proc finish {} {
        global ns nf
        $ns flush-trace
        close $nf       
        exec nam out.nam &
        exit 0
}

#Defining Four Nodes
set n0 [$ns node]
set n1 [$ns node]
set n2 [$ns node]
set n3 [$ns node]

#Defining Duplex link between nodes having bandwidth of 2Mbps, delay of 10ms and Droptail Queue
$ns duplex-link $n0 $n2 2Mb 10ms DropTail

#Defining Duplex link between nodes having bandwidth of 2Mbps, delay of 10ms and Droptail Queue
$ns duplex-link $n1 $n2 2Mb 10ms DropTail

#Defining Duplex link between nodes having bandwidth of 1.7Mbps, delay of 20ms and Droptail Queue
$ns duplex-link $n2 $n3 1.7Mb 20ms DropTail

#Set Queue Size of link (n2-n3) to 10
$ns queue-limit $n2 $n3 10

#Orienting the nodes for proper visulaization
#n2 will be on the right-down position w.r.t n0
$ns duplex-link-op $n0 $n2 orient right-down

#Orienting the nodes for proper visulaization
#n2 will be on the right-up position w.r.t n1
$ns duplex-link-op $n1 $n2 orient right-up

#Orienting the nodes for proper visulaization
#n3 will be on the right position w.r.t n2
$ns duplex-link-op $n2 $n3 orient right

#Creating a TCP agent and connecting it to n0 (Basically it defines source node of TCP)
set tcp0 [new Agent/TCP]
$ns attach-agent $n0 $tcp0

#Creating a Sink Agent and attaching it to n3
set sink0 [new Agent/TCPSink]
$ns attach-agent $n3 $sink0

#Connecting TCP agent with Sink agent
$ns connect $tcp0 $sink0

#Specifying tcp traffic to have blue color as defined in the second line of program
$tcp0 set fid_ 0

#Creating FTP agent for traffic and attching it to tcp0
set ftp0 [new Application/FTP]
$ftp0 attach-agent $tcp0


#Specifying the UDP agent
set udp0 [new Agent/UDP]
#Attaching the UDP agent with n1
$ns attach-agent $n1 $udp0

#Specifying the Null agent
set null0 [new Agent/Null]
#Attaching the NULL agent with n3
$ns attach-agent $n3 $null0

#Connecting both udp0 and null0 agents for transferring data between n3 and n1
$ns connect $udp0 $null0

#Specifying udp traffic to have red color as defined in the second line of program
$udp0 set fid_ 1

#Specifying the CBR agent to generate the traffic over udp0 agent
set cbr0 [new Application/Traffic/CBR]

#Attaching cbr0 with udp0
$cbr0 attach-agent $udp0
#$cbr0 set type_ CBR

#Each packet having 1000 bytes
$cbr0 set packet_size_ 100

#Each packet having mb rate
$cbr0 set rate_ 1kb
$cbr0 set random_ false

#Starting the cbr0 at 0.1 simulation time
$ns at 0.1 "$cbr0 start"
#Starting the ftp0 at 1.0 simulation time
$ns at 1.0 "$ftp0 start"
#Stoping the ftp0 at 4.0 simulation time
$ns at 4.0 "$ftp0 stop"
#Stoping the cbr0 at 4.5 simulation time
$ns at 4.5 "$cbr0 stop"

#Calling the finish procedure
$ns at 5.0 "finish"


#Run the simulation
$ns run


