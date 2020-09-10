FileNames = ['A_10%.txt'
             'A_20%.txt'
             'A_30%.txt'
             'A_40%.txt'
             'A_50%.txt'
             'A_60%.txt'
             'A_70%.txt'
             'A_80%.txt'
             'A_90%.txt'];

ave = zeros(9,40);
max = zeros(9,40);
win = zeros(9,40);

for i = (1:9)
    FileName = FileNames(i,:);
    all  = strsplit(fileread(FileName), '\n');
    table = reshape(all,[8,40]);
    ave_ = str2double(erase(table(2,:),'Ave score: '));
    max_ = str2double(erase(table(3,:),'Max score: '));
    win_ = str2double(erase(table(7,:),'Win rate: '));
    ave(i,:) = ave_;
    max(i,:) = max_;
    win(i,:) = win_;
end

change = 20000;

rounds = (1:40)*1000;
legends = ['A_1_0'; 'A_2_0'; 'A_3_0'; 'A_4_0'; 'A_5_0'; 'A_6_0'; 'A_7_0'; 'A_8_0'; 'A_9_0'];

f1= figure('Name','Ave score');
plot(rounds,ave,'-');
xline(change, '--');
title('4,6-tuple E_1_0: Ave score vs Rounds');
xlabel('Rounds');
legend(legends, 'location', 'southeast');
saveas(f1, 'Ave score.jpg');

f2= figure('Name','Max score');
plot(rounds,max,'-');
xline(change, '--');
title('4,6-tuple E_1_0: Max score vs Rounds');
xlabel('Rounds');
legend(legends, 'location', 'southeast');
saveas(f2, 'Max score.jpg');

f3= figure('Name','Win rate');
plot(rounds,win,'-');
xline(change, '--');
title('4,6-tuple E_1_0: Win rate vs Rounds');
xlabel('Rounds');
legend(legends, 'location', 'southeast');
saveas(f3, 'Win rate.jpg');

