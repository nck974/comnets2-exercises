function [  ] = plot_histogram( )
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%Plot results of exercise 3.
%Place in the same folder as .vec files (results), check names and execute.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%



%MersennetwisterRNG
figure(1)
data1 = importdata('MersennetwisterRNG-0.vec');
vector1=data1.data(:,4);
Histg1=hist(vector1);
plot(Histg1/numel(data1.data),'--go');  
clear data1;
hold on

%LinearCongruentialRNG
data2 = importdata('LinearCongruentialRNG-0.vec');
vector2=data2.data(:,4);
Histg2=hist(vector2);
plot(Histg2/numel(data2.data),':r*');  
clear data2;

xlabel('Interarrival time');
ylabel('Probability');
legend('Mersenne Twister RNG','Linear Congruential RNG','Location','northeast');


end

