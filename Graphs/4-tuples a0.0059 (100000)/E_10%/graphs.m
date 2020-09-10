FileNames1 = ['A_10%.txt'
             'A_20%.txt'
             'A_30%.txt'
             'A_40%.txt'
             'A_50%.txt'
             'A_60%.txt'
             'A_70%.txt'
             'A_80%.txt'
             'A_90%.txt'];
         
FileNames2 = ['E_10%.txt'
             'E_20%.txt'
             'E_30%.txt'
             'E_40%.txt'
             'E_50%.txt'
             'E_60%.txt'
             'E_70%.txt'
             'E_80%.txt'
             'E_90%.txt'];


ave = zeros(9,200);
max = zeros(9,200);
win = zeros(9,200);

for i = (1:9)
    FileName = FileNames1(i,:);
    all  = strsplit(fileread(FileName), '\n');
    table = reshape(all,[8,100]);
    ave_ = str2double(erase(table(2,:),'Ave score: '));
    max_ = str2double(erase(table(3,:),'Max score: '));
    win_ = str2double(erase(table(7,:),'Win rate: '));
    ave(i,101:200) = ave_;
    max(i,101:200) = max_;
    win(i,101:200) = win_;
    FileName = FileNames2(i,:);
    all  = strsplit(fileread(FileName), '\n');
    table = reshape(all,[8,100]);
    ave_ = str2double(erase(table(2,:),'Ave score: '));
    max_ = str2double(erase(table(3,:),'Max score: '));
    win_ = str2double(erase(table(7,:),'Win rate: '));
    ave(i,1:100) = ave_;
    max(i,1:100) = max_;
    win(i,1:100) = win_;
end

change = 100000;

rounds = (1:200)*1000;
legends = ['A_1_0'; 'A_2_0'; 'A_3_0'; 'A_4_0'; 'A_5_0'; 'A_6_0'; 'A_7_0'; 'A_8_0'; 'A_9_0'];

f1= figure('Name','Ave score');
plot(rounds,ave,'-');
xline(change, '--');
title('4-tuple E_1_0: Ave score vs Rounds');
xlabel('Rounds');
legend(legends, 'location', 'southeast');
saveas(f1, 'Ave score.jpg');

f2= figure('Name','Max score');
plot(rounds,max,'-');
xline(change, '--');
title('4-tuple E_1_0: Max score vs Rounds');
xlabel('Rounds');
legend(legends, 'location', 'southeast');
saveas(f2, 'Max score.jpg');

f3= figure('Name','Win rate');
plot(rounds,win,'-');
xline(change, '--');
title('4-tuple E_1_0: Win rate vs Rounds');
xlabel('Rounds');
legend(legends, 'location', 'southeast');
saveas(f3, 'Win rate.jpg');

