// StripController.h

#ifndef _ANALYZER_h
#define _ANALYZER_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "Predefined.h"

class Analyzer
    {
    private:
        void measureVol ();

        float volume_filt = 0;
        float freq_max_filt = 0;
        float freq_filt [SPECTRUM_SIZE];
        float freq_peaks_filt [3];

    public:
        Analyzer ();
        
        void update ();

    };


// VU
/*
if (this_mode == 0 || this_mode == 1) 
{
for (byte i = 0; i < 100; i ++)
{
RcurrentLevel = analogRead(SOUND_R);
if (RsoundLevel < RcurrentLevel) RsoundLevel = RcurrentLevel;

}

// ��������� �� ������� ������ �����
RsoundLevel = map(RsoundLevel, LOW_PASS, 1023, 0, 500);
if (!MONO)LsoundLevel = map(LsoundLevel, LOW_PASS, 1023, 0, 500);

// ������������ ��������
RsoundLevel = constrain(RsoundLevel, 0, 500);
if (!MONO)LsoundLevel = constrain(LsoundLevel, 0, 500);

// �������� � ������� (��� ������� �������� ������)
RsoundLevel = pow(RsoundLevel, EXP);
if (!MONO)LsoundLevel = pow(LsoundLevel, EXP);

// ������
RsoundLevel_f = RsoundLevel * SMOOTH + RsoundLevel_f * (1 - SMOOTH);
if (!MONO)LsoundLevel_f = LsoundLevel * SMOOTH + LsoundLevel_f * (1 - SMOOTH);

if (MONO) LsoundLevel_f = RsoundLevel_f;  // ���� ����, �� ����� = �������

// �������� "��������", ���� ������� �����������
if (EMPTY_BRIGHT > 5) {
for (int i = 0; i < NUM_LEDS; i++)
leds[i] = CHSV(EMPTY_COLOR, 255, EMPTY_BRIGHT);
}

// ���� �������� ���� ������ - �������� ����� ����������
if (RsoundLevel_f > 15 && LsoundLevel_f > 15) {

// ������ ����� ������� ��������� � ����� �������, ����������.
// ������ ����� ���������, ������� ���������� ��� �������������
averageLevel = (float)(RsoundLevel_f + LsoundLevel_f) / 2 * averK + averageLevel * (1 - averK);

// ��������� ������������ ��������� ����� ��� �������, ���������� �� ��������� ����������� MAX_COEF
maxLevel = (float)averageLevel * MAX_COEF;

// ����������� ������ � ����� ����� (��� MAX_CH ��� �������� ���������� �����������)
Rlenght = map(RsoundLevel_f, 0, maxLevel, 0, MAX_CH);
Llenght = map(LsoundLevel_f, 0, maxLevel, 0, MAX_CH);

// ������������ �� ����. ����� �����������
Rlenght = constrain(Rlenght, 0, MAX_CH);
Llenght = constrain(Llenght, 0, MAX_CH);

animation();       // ����������
}
}

*/

// freq
/*
// 3-5 ����� - �����������
if (this_mode == 2 || this_mode == 3 || this_mode == 4 || this_mode == 7 || this_mode == 8) {
analyzeAudio();
colorMusic[0] = 0;
colorMusic[1] = 0;
colorMusic[2] = 0;
for (int i = 0 ; i < 32 ; i++) {
if (fht_log_out[i] < SPEKTR_LOW_PASS) fht_log_out[i] = 0;
}
// ������ �������, ������� �� 2 �� 5 ��� (0 � 1 �����������!)
for (byte i = 2; i < 6; i++) {
if (fht_log_out[i] > colorMusic[0]) colorMusic[0] = fht_log_out[i];
}
// ������� �������, ������� � 6 �� 10 ���
for (byte i = 6; i < 11; i++) {
if (fht_log_out[i] > colorMusic[1]) colorMusic[1] = fht_log_out[i];
}
// ������� �������, ������� � 11 �� 31 ���
for (byte i = 11; i < 32; i++) {
if (fht_log_out[i] > colorMusic[2]) colorMusic[2] = fht_log_out[i];
}
freq_max = 0;
for (byte i = 0; i < 30; i++) {
if (fht_log_out[i + 2] > freq_max) freq_max = fht_log_out[i + 2];
if (freq_max < 5) freq_max = 5;

if (freq_f[i] < fht_log_out[i + 2]) freq_f[i] = fht_log_out[i + 2];
if (freq_f[i] > 0) freq_f[i] -= LIGHT_SMOOTH;
else freq_f[i] = 0;
}
freq_max_f = freq_max * averK + freq_max_f * (1 - averK);
for (byte i = 0; i < 3; i++) {
colorMusic_aver[i] = colorMusic[i] * averK + colorMusic_aver[i] * (1 - averK);  // ����� ����������
colorMusic_f[i] = colorMusic[i] * SMOOTH_FREQ + colorMusic_f[i] * (1 - SMOOTH_FREQ);      // ���������
if (colorMusic_f[i] > ((float)colorMusic_aver[i] * MAX_COEF_FREQ)) {
thisBright[i] = 255;
colorMusicFlash[i] = true;
running_flag[i] = true;
} else colorMusicFlash[i] = false;
if (thisBright[i] >= 0) thisBright[i] -= SMOOTH_STEP;
if (thisBright[i] < EMPTY_BRIGHT) {
thisBright[i] = EMPTY_BRIGHT;
running_flag[i] = false;
}
}
animation();
}

*/

#endif