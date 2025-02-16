% Analyses WAV File
clc
clear all

filename = ".\funnysong_filtered.wav";
[signal,Fs]=audioread(filename);
Amax = max(signal);