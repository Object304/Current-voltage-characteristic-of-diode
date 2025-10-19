clc, clear, close all

data = load('output.txt');
x = data(:, 1);
y = data(:, 2);

plot(x, y, '.');