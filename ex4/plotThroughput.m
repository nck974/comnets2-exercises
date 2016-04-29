function [  ] = plotThroughput(  )

%Throughpugt in packets per sec
figure(1)
subplot(2,1,1)
data_vec=importdata('Default-0.vec'); %Default
data=data_vec.data(:,1);
plot(data,'r--');

hold on

data_vec2=importdata('MoreDataRate-0.vec'); %Datarate
data2=data_vec2.data(:,1);
plot(data2,'g');


hold on

data_vec3=importdata('MoreDelay-0.vec'); %Delay
data3=data_vec3.data(:,1);
plot(data3,'b--');


hold on

data_vec4=importdata('MoreBER-0.vec'); %BER
data4=data_vec4.data(:,1);
plot(data4,'m--');

xlabel('Time');
ylabel('Client Throughput [packets/sec]');
legend('Default','More Data Rate','More Delay','More BER','Location','northeast');





%Throughput in bits per sec
subplot(2,1,2)
data=data_vec.data(:,2); %Default
plot(data,'r--');

hold on

data2=data_vec2.data(:,2); %Datarate
plot(data2,'g--');


hold on

data3=data_vec3.data(:,2); %Delay
plot(data3,'b--');


hold on

data4=data_vec4.data(:,2); %BER
plot(data4,'m--');

xlabel('Time');
ylabel('Client Throughput [bits/sec]');
legend('Default','More Data Rate','More Delay','More BER','Location','northeast');



end

