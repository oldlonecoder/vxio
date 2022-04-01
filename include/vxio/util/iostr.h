//
// Created by oldlonecoder on 21-11-20.
//

//#ifndef VXIO_FRAMEWORK_IOSTR_H
//#define VXIO_FRAMEWORK_IOSTR_H
#pragma once

#include <iostream>
#include <cstring>
#include <sstream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <ctime>
#include <cstdint>
#include <cmath>
#include <functional>
#include <string>
#include <string_view>
#include <vxio/dll_config.h>

namespace vxio
{
struct VXIO_API color
{
    enum type : uint16_t
    {
        /*0   */          Black,//#000000	rgb(128,0,0)	hsl(0,100%,25%)
        /*1   */          Maroon,//#800000	rgb(128,0,0)	hsl(0,100%,25%)
        /*2   */          Green,//#008000	rgb(0,128,0)	hsl(120,100%,25%)
        /*3   */          Olive,//#808000	rgb(128,128,0)	hsl(60,100%,25%)
        /*4   */          Navy,//#000080	rgb(0,0,128)	hsl(240,100%,25%)
        /*5   */          Purple,//#800080	rgb(128,0,128)	hsl(300,100%,25%)
        /*6   */          Teal,//#008080	rgb(0,128,128)	hsl(180,100%,25%)
        /*7   */          Silver,//#c0c0c0	rgb(192,192,192)	hsl(0,0%,75%)
        /*8   */          Grey,//#808080	rgb(128,128,128)	hsl(0,0%,50%)
        /*9   */          Red,//#ff0000	rgb(255,0,0)	hsl(0,100%,50%)
        /*10  */          Lime,//#00ff00	rgb(0,255,0)	hsl(120,100%,50%)
        /*11  */          Yellow,//#ffff00	rgb(255,255,0)	hsl(60,100%,50%)
        /*12  */          Blue,//#0000ff	rgb(0,0,255)	hsl(240,100%,50%)
        /*13  */          Fuchsia,//#ff00ff	rgb(255,0,255)	hsl(300,100%,50%)
        /*14  */          Aqua,//#00ffff	rgb(0,255,255)	hsl(180,100%,50%)
        /*15  */          White,//#ffffff	rgb(255,255,255)	hsl(0,0%,100%)
        /*16  */          Grey0,//#000000	rgb(0,0,0)	hsl(0,0%,0%)
        /*17  */          NavyBlue,//#00005f	rgb(0,0,95)	hsl(240,100%,18%)
        /*18  */          DarkBlue,//#000087	rgb(0,0,135)	hsl(240,100%,26%)
        /*19  */          Blue3,//#0000af	rgb(0,0,175)	hsl(240,100%,34%)
        /*20  */          Blue4,//#0000d7	rgb(0,0,215)	hsl(240,100%,42%)
        /*21  */          Blue1,//#0000ff	rgb(0,0,255)	hsl(240,100%,50%)
        /*22  */          DarkGreen,//#005f00	rgb(0,95,0)	hsl(120,100%,18%)
        /*23  */          DeepSkyBlue4,//#005f5f	rgb(0,95,95)	hsl(180,100%,18%)
        /*24  */          DeepSkyBlue5,//#005f87	rgb(0,95,135)	hsl(97,100%,26%)
        /*25  */          DeepSkyBlue6,//#005faf	rgb(0,95,175)	hsl(07,100%,34%)
        /*26  */          DodgerBlue3,//#005fd7	rgb(0,95,215)	hsl(13,100%,42%)
        /*27  */          DodgerBlue2,//#005fff	rgb(0,95,255)	hsl(17,100%,50%)
        /*28  */          Green4,//#008700	rgb(0,135,0)	hsl(120,100%,26%)
        /*29  */          SpringGreen4,//#00875f	rgb(0,135,95)	hsl(62,100%,26%)
        /*30  */          Turquoise4,//#008787	rgb(0,135,135)	hsl(180,100%,26%)
        /*31  */          DeepSkyBlue3,//#0087af	rgb(0,135,175)	hsl(93,100%,34%)
        /*32  */          DeepSkyBlue7,//#0087d7	rgb(0,135,215)	hsl(02,100%,42%)
        /*33  */          DodgerBlue1,//#0087ff	rgb(0,135,255)	hsl(08,100%,50%)
        /*34  */          Green3,//#00af00	rgb(0,175,0)	hsl(120,100%,34%)
        /*35  */          SpringGreen3,//#00af5f	rgb(0,175,95)	hsl(52,100%,34%)
        /*36  */          DarkCyan,//#00af87	rgb(0,175,135)	hsl(66,100%,34%)
        /*37  */          LightSeaGreen,//#00afaf	rgb(0,175,175)	hsl(180,100%,34%)
        /*38  */          DeepSkyBlue2,//#00afd7	rgb(0,175,215)	hsl(91,100%,42%)
        /*39  */          DeepSkyBlue1,//#00afff	rgb(0,175,255)	hsl(98,100%,50%)
        /*40  */          Green5,//#00d700	rgb(0,215,0)	hsl(120,100%,42%)
        /*41  */          SpringGreen5,//#00d75f	rgb(0,215,95)	hsl(46,100%,42%)
        /*42  */          SpringGreen2,//#00d787	rgb(0,215,135)	hsl(57,100%,42%)
        /*43  */          Cyan3,//#00d7af	rgb(0,215,175)	hsl(68,100%,42%)
        /*44  */          DarkTurquoise,//#00d7d7	rgb(0,215,215)	hsl(180,100%,42%)
        /*45  */          Turquoise2,//#00d7ff	rgb(0,215,255)	hsl(89,100%,50%)
        /*46  */          Green1,//#00ff00	rgb(0,255,0)	hsl(120,100%,50%)
        /*47  */          SpringGreen6,//#00ff5f	rgb(0,255,95)	hsl(42,100%,50%)
        /*48  */          SpringGreen1,//#00ff87	rgb(0,255,135)	hsl(51,100%,50%)
        /*49  */          MediumSpringGreen,//#00ffaf	rgb(0,255,175)	hsl(61,100%,50%)
        /*50  */          Cyan2,//#00ffd7	rgb(0,255,215)	hsl(70,100%,50%)
        /*51  */          Cyan1,//#00ffff	rgb(0,255,255)	hsl(180,100%,50%)
        /*52  */          DarkRed,//#5f0000	rgb(95,0,0)	hsl(0,100%,18%)
        /*53  */          DeepPink4,//#5f005f	rgb(95,0,95)	hsl(300,100%,18%)
        /*54  */          Purple4,//#5f0087	rgb(95,0,135)	hsl(82,100%,26%)
        /*55  */          Purple5,//#5f00af	rgb(95,0,175)	hsl(72,100%,34%)
        /*56  */          Purple3,//#5f00d7	rgb(95,0,215)	hsl(66,100%,42%)
        /*57  */          BlueViolet,//#5f00ff	rgb(95,0,255)	hsl(62,100%,50%)
        /*58  */          Orange4,//#5f5f00	rgb(95,95,0)	hsl(60,100%,18%)
        /*59  */          Grey37,//#5f5f5f	rgb(95,95,95)	hsl(0,0%,37%)
        /*60  */          MediumPurple4,//#5f5f87	rgb(95,95,135)	hsl(240,17%,45%)
        /*61  */          SlateBlue3,//#5f5faf	rgb(95,95,175)	hsl(240,33%,52%)
        /*62  */          SlateBlue4,//#5f5fd7	rgb(95,95,215)	hsl(240,60%,60%)
        /*63  */          RoyalBlue1,//#5f5fff	rgb(95,95,255)	hsl(240,100%,68%)
        /*64  */          Chartreuse4,//#5f8700	rgb(95,135,0)	hsl(7,100%,26%)
        /*65  */          DarkSeaGreen4,//#5f875f	rgb(95,135,95)	hsl(120,17%,45%)
        /*66  */          PaleTurquoise4,//#5f8787	rgb(95,135,135)	hsl(180,17%,45%)
        /*67  */          SteelBlue,//#5f87af	rgb(95,135,175)	hsl(210,33%,52%)
        /*68  */          SteelBlue3,//#5f87d7	rgb(95,135,215)	hsl(220,60%,60%)
        /*69  */          CornflowerBlue,//#5f87ff	rgb(95,135,255)	hsl(225,100%,68%)
        /*70  */          Chartreuse3,//#5faf00	rgb(95,175,0)	hsl(7,100%,34%)
        /*71  */          DarkSeaGreen5,//#5faf5f	rgb(95,175,95)	hsl(120,33%,52%)
        /*72  */          CadetBlue,//#5faf87	rgb(95,175,135)	hsl(150,33%,52%)
        /*73  */          CadetBlue2,//#5fafaf	rgb(95,175,175)	hsl(180,33%,52%)
        /*74  */          SkyBlue3,//#5fafd7	rgb(95,175,215)	hsl(200,60%,60%)
        /*75  */          SteelBlue1,//#5fafff	rgb(95,175,255)	hsl(210,100%,68%)
        /*76  */          Chartreuse5,//#5fd700	rgb(95,215,0)	hsl(3,100%,42%)
        /*77  */          PaleGreen3,//#5fd75f	rgb(95,215,95)	hsl(120,60%,60%)
        /*78  */          SeaGreen3,//#5fd787	rgb(95,215,135)	hsl(140,60%,60%)
        /*79  */          Aquamarine3,//#5fd7af	rgb(95,215,175)	hsl(160,60%,60%)
        /*80  */          MediumTurquoise,//#5fd7d7	rgb(95,215,215)	hsl(180,60%,60%)
        /*81  */          SteelBlue4,//#5fd7ff	rgb(95,215,255)	hsl(195,100%,68%)
        /*82  */          Chartreuse2,//#5fff00	rgb(95,255,0)	hsl(7,100%,50%)
        /*83  */          SeaGreen2,//#5fff5f	rgb(95,255,95)	hsl(120,100%,68%)
        /*84  */          SeaGreen1,//#5fff87	rgb(95,255,135)	hsl(135,100%,68%)
        /*85  */          SeaGreen4,//#5fffaf	rgb(95,255,175)	hsl(150,100%,68%)
        /*86  */          Aquamarine1,//#5fffd7	rgb(95,255,215)	hsl(165,100%,68%)
        /*87  */          DarkSlateGray2,//#5fffff	rgb(95,255,255)	hsl(180,100%,68%)
        /*88  */          DarkRed2,//#870000	rgb(135,0,0)	hsl(0,100%,26%)
        /*89  */          DeepPink5,//#87005f	rgb(135,0,95)	hsl(17,100%,26%)
        /*90  */          DarkMagenta,//#870087	rgb(135,0,135)	hsl(300,100%,26%)
        /*91  */          DarkMagenta2,//#8700af	rgb(135,0,175)	hsl(86,100%,34%)
        /*92  */          DarkViolet,//#8700d7	rgb(135,0,215)	hsl(77,100%,42%)
        /*93  */          Purple2,//#8700ff	rgb(135,0,255)	hsl(71,100%,50%)
        /*94  */          Orange5,//#875f00	rgb(135,95,0)	hsl(2,100%,26%)
        /*95  */          LightPink4,//#875f5f	rgb(135,95,95)	hsl(0,17%,45%)
        /*96  */          Plum4,//#875f87	rgb(135,95,135)	hsl(300,17%,45%)
        /*97  */          MediumPurple3,//#875faf	rgb(135,95,175)	hsl(270,33%,52%)
        /*98  */          MediumPurple5,//#875fd7	rgb(135,95,215)	hsl(260,60%,60%)
        /*99  */          SlateBlue1,//#875fff	rgb(135,95,255)	hsl(255,100%,68%)
        /*100 */          Yellow4,//#878700	rgb(135,135,0)	hsl(60,100%,26%)
        /*101 */          Wheat4,//#87875f	rgb(135,135,95)	hsl(60,17%,45%)
        /*102 */          Grey53,//#878787	rgb(135,135,135)	hsl(0,0%,52%)
        /*103 */          LightSlateGrey,//#8787af	rgb(135,135,175)	hsl(240,20%,60%)
        /*104 */          MediumPurple,//#8787d7	rgb(135,135,215)	hsl(240,50%,68%)
        /*105 */          LightSlateBlue,//#8787ff	rgb(135,135,255)	hsl(240,100%,76%)
        /*106 */          Yellow5,//#87af00	rgb(135,175,0)	hsl(3,100%,34%)
        /*107 */          DarkOliveGreen3,//#87af5f	rgb(135,175,95)	hsl(90,33%,52%)
        /*108 */          DarkSeaGreen,//#87af87	rgb(135,175,135)	hsl(120,20%,60%)
        /*109 */          LightSkyBlue3,//#87afaf	rgb(135,175,175)	hsl(180,20%,60%)
        /*110 */          LightSkyBlue4,//#87afd7	rgb(135,175,215)	hsl(210,50%,68%)
        /*111 */          SkyBlue2,//#87afff	rgb(135,175,255)	hsl(220,100%,76%)
        /*112 */          Chartreuse6,//#87d700	rgb(135,215,0)	hsl(2,100%,42%)
        /*113 */          DarkOliveGreen4,//#87d75f	rgb(135,215,95)	hsl(100,60%,60%)
        /*114 */          PaleGreen4,//#87d787	rgb(135,215,135)	hsl(120,50%,68%)
        /*115 */          DarkSeaGreen3,//#87d7af	rgb(135,215,175)	hsl(150,50%,68%)
        /*116 */          DarkSlateGray3,//#87d7d7	rgb(135,215,215)	hsl(180,50%,68%)
        /*117 */          SkyBlue1,//#87d7ff	rgb(135,215,255)	hsl(200,100%,76%)
        /*118 */          Chartreuse1,//#87ff00	rgb(135,255,0)	hsl(8,100%,50%)
        /*119 */          LightGreen,//#87ff5f	rgb(135,255,95)	hsl(105,100%,68%)
        /*120 */          LightGreen2,//#87ff87	rgb(135,255,135)	hsl(120,100%,76%)
        /*121 */          PaleGreen1,//#87ffaf	rgb(135,255,175)	hsl(140,100%,76%)
        /*122 */          Aquamarine2,//#87ffd7	rgb(135,255,215)	hsl(160,100%,76%)
        /*123 */          DarkSlateGray1,//#87ffff	rgb(135,255,255)	hsl(180,100%,76%)
        /*124 */          Red3,//#af0000	rgb(175,0,0)	hsl(0,100%,34%)
        /*125 */          DeepPink6,//#af005f	rgb(175,0,95)	hsl(27,100%,34%)
        /*126 */          MediumVioletRed,//#af0087	rgb(175,0,135)	hsl(13,100%,34%)
        /*127 */          Magenta3,//#af00af	rgb(175,0,175)	hsl(300,100%,34%)
        /*128 */          DarkViole2,//#af00d7	rgb(175,0,215)	hsl(88,100%,42%)
        /*129 */          Purple6,//#af00ff	rgb(175,0,255)	hsl(81,100%,50%)
        /*130 */          DarkOrange3,//#af5f00	rgb(175,95,0)	hsl(2,100%,34%)
        /*131 */          IndianRed,//#af5f5f	rgb(175,95,95)	hsl(0,33%,52%)
        /*132 */          HotPink3,//#af5f87	rgb(175,95,135)	hsl(330,33%,52%)
        /*133 */          MediumOrchid3,//#af5faf	rgb(175,95,175)	hsl(300,33%,52%)
        /*134 */          MediumOrchid,//#af5fd7	rgb(175,95,215)	hsl(280,60%,60%)
        /*135 */          MediumPurple2,//#af5fff	rgb(175,95,255)	hsl(270,100%,68%)
        /*136 */          DarkGoldenrod,//#af8700	rgb(175,135,0)	hsl(6,100%,34%)
        /*137 */          LightSalmon3,//#af875f	rgb(175,135,95)	hsl(30,33%,52%)
        /*138 */          RosyBrown,//#af8787	rgb(175,135,135)	hsl(0,20%,60%)
        /*139 */          Grey63,//#af87af	rgb(175,135,175)	hsl(300,20%,60%)
        /*140 */          MediumPurple6,//#af87d7	rgb(175,135,215)	hsl(270,50%,68%)
        /*141 */          MediumPurple1,//#af87ff	rgb(175,135,255)	hsl(260,100%,76%)
        /*142 */          Gold3,//#afaf00	rgb(175,175,0)	hsl(60,100%,34%)
        /*143 */          DarkKhaki,//#afaf5f	rgb(175,175,95)	hsl(60,33%,52%)
        /*144 */          NavajoWhite3,//#afaf87	rgb(175,175,135)	hsl(60,20%,60%)
        /*145 */          Grey69,//#afafaf	rgb(175,175,175)	hsl(0,0%,68%)
        /*146 */          LightSteelBlue3,//#afafd7	rgb(175,175,215)	hsl(240,33%,76%)
        /*147 */          LightSteelBlue,//#afafff	rgb(175,175,255)	hsl(240,100%,84%)
        /*148 */          Yellow3,//#afd700	rgb(175,215,0)	hsl(1,100%,42%)
        /*149 */          DarkOliveGreen5,//#afd75f	rgb(175,215,95)	hsl(80,60%,60%)
        /*150 */          DarkSeaGreen6,//#afd787	rgb(175,215,135)	hsl(90,50%,68%)
        /*151 */          DarkSeaGreen2,//#afd7af	rgb(175,215,175)	hsl(120,33%,76%)
        /*152 */          LightCyan3,//#afd7d7	rgb(175,215,215)	hsl(180,33%,76%)
        /*153 */          LightSkyBlue1,//#afd7ff	rgb(175,215,255)	hsl(210,100%,84%)
        /*154 */          GreenYellow,//#afff00	rgb(175,255,0)	hsl(8,100%,50%)
        /*155 */          DarkOliveGreen2,//#afff5f	rgb(175,255,95)	hsl(90,100%,68%)
        /*156 */          PaleGreen2,//#afff87	rgb(175,255,135)	hsl(100,100%,76%)
        /*157 */          DarkSeaGreen7,//#afffaf	rgb(175,255,175)	hsl(120,100%,84%)
        /*158 */          DarkSeaGreen1,//#afffd7	rgb(175,255,215)	hsl(150,100%,84%)
        /*159 */          PaleTurquoise1,//#afffff	rgb(175,255,255)	hsl(180,100%,84%)
        /*160 */          Red4,//#d70000	rgb(215,0,0)	hsl(0,100%,42%)
        /*161 */          DeepPink3,//#d7005f	rgb(215,0,95)	hsl(33,100%,42%)
        /*162 */          DeepPink7,//#d70087	rgb(215,0,135)	hsl(22,100%,42%)
        /*163 */          Magenta4,//#d700af	rgb(215,0,175)	hsl(11,100%,42%)
        /*164 */          Magenta5,//#d700d7	rgb(215,0,215)	hsl(300,100%,42%)
        /*165 */          Magenta2,//#d700ff	rgb(215,0,255)	hsl(90,100%,50%)
        /*166 */          DarkOrange4,//#d75f00	rgb(215,95,0)	hsl(6,100%,42%)
        /*167 */          IndianRed2,//#d75f5f	rgb(215,95,95)	hsl(0,60%,60%)
        /*168 */          HotPink4,//#d75f87	rgb(215,95,135)	hsl(340,60%,60%)
        /*169 */          HotPink2,//#d75faf	rgb(215,95,175)	hsl(320,60%,60%)
        /*170 */          Orchid,//#d75fd7	rgb(215,95,215)	hsl(300,60%,60%)
        /*171 */          MediumOrchid1,//#d75fff	rgb(215,95,255)	hsl(285,100%,68%)
        /*172 */          Orange3,//#d78700	rgb(215,135,0)	hsl(7,100%,42%)
        /*173 */          LightSalmon4,//#d7875f	rgb(215,135,95)	hsl(20,60%,60%)
        /*174 */          LightPink3,//#d78787	rgb(215,135,135)	hsl(0,50%,68%)
        /*175 */          Pink3,//#d787af	rgb(215,135,175)	hsl(330,50%,68%)
        /*176 */          Plum3,//#d787d7	rgb(215,135,215)	hsl(300,50%,68%)
        /*177 */          Violet,//#d787ff	rgb(215,135,255)	hsl(280,100%,76%)
        /*178 */          Gold4,//#d7af00	rgb(215,175,0)	hsl(8,100%,42%)
        /*179 */          LightGoldenrod3,//#d7af5f	rgb(215,175,95)	hsl(40,60%,60%)
        /*180 */          Tan,//#d7af87	rgb(215,175,135)	hsl(30,50%,68%)
        /*181 */          MistyRose3,//#d7afaf	rgb(215,175,175)	hsl(0,33%,76%)
        /*182 */          Thistle3,//#d7afd7	rgb(215,175,215)	hsl(300,33%,76%)
        /*183 */          Plum2,//#d7afff	rgb(215,175,255)	hsl(270,100%,84%)
        /*184 */          Yellow6,//#d7d700	rgb(215,215,0)	hsl(60,100%,42%)
        /*185 */          Khaki3,//#d7d75f	rgb(215,215,95)	hsl(60,60%,60%)
        /*186 */          LightGoldenrod2,//#d7d787	rgb(215,215,135)	hsl(60,50%,68%)
        /*187 */          LightYellow3,//#d7d7af	rgb(215,215,175)	hsl(60,33%,76%)
        /*188 */          Grey84,//#d7d7d7	rgb(215,215,215)	hsl(0,0%,84%)
        /*189 */          LightSteelBlue1,//#d7d7ff	rgb(215,215,255)	hsl(240,100%,92%)
        /*190 */          Yellow2,//#d7ff00	rgb(215,255,0)	hsl(9,100%,50%)
        /*191 */          DarkOliveGreen1,//#d7ff5f	rgb(215,255,95)	hsl(75,100%,68%)
        /*192 */          DarkOliveGreen6,//#d7ff87	rgb(215,255,135)	hsl(80,100%,76%)
        /*193 */          DarkSeaGreen8,//#d7ffaf	rgb(215,255,175)	hsl(90,100%,84%)
        /*194 */          Honeydew2,//#d7ffd7	rgb(215,255,215)	hsl(120,100%,92%)
        /*195 */          LightCyan1,//#d7ffff	rgb(215,255,255)	hsl(180,100%,92%)
        /*196 */          Red1,//#ff0000	rgb(255,0,0)	hsl(0,100%,50%)
        /*197 */          DeepPink2,//#ff005f	rgb(255,0,95)	hsl(37,100%,50%)
        /*198 */          DeepPink1,//#ff0087	rgb(255,0,135)	hsl(28,100%,50%)
        /*199 */          DeepPink8,//#ff00af	rgb(255,0,175)	hsl(18,100%,50%)
        /*200 */          Magenta6,//#ff00d7	rgb(255,0,215)	hsl(09,100%,50%)
        /*201 */          Magenta1,//#ff00ff	rgb(255,0,255)	hsl(300,100%,50%)
        /*202 */          OrangeRed1,//#ff5f00	rgb(255,95,0)	hsl(2,100%,50%)
        /*203 */          IndianRed1,//#ff5f5f	rgb(255,95,95)	hsl(0,100%,68%)
        /*204 */          IndianRed3,//#ff5f87	rgb(255,95,135)	hsl(345,100%,68%)
        /*205 */          HotPink,//#ff5faf	rgb(255,95,175)	hsl(330,100%,68%)
        /*206 */          HotPin2,//#ff5fd7	rgb(255,95,215)	hsl(315,100%,68%)
        /*207 */          MediumOrchid2,//#ff5fff	rgb(255,95,255)	hsl(300,100%,68%)
        /*208 */          DarkOrange,//#ff8700	rgb(255,135,0)	hsl(1,100%,50%)
        /*209 */          Salmon1,//#ff875f	rgb(255,135,95)	hsl(15,100%,68%)
        /*210 */          LightCoral,//#ff8787	rgb(255,135,135)	hsl(0,100%,76%)
        /*211 */          PaleVioletRed1,//#ff87af	rgb(255,135,175)	hsl(340,100%,76%)
        /*212 */          Orchid2,//#ff87d7	rgb(255,135,215)	hsl(320,100%,76%)
        /*213 */          Orchid1,//#ff87ff	rgb(255,135,255)	hsl(300,100%,76%)
        /*214 */          Orange1,//#ffaf00	rgb(255,175,0)	hsl(1,100%,50%)
        /*215 */          SandyBrown,//#ffaf5f	rgb(255,175,95)	hsl(30,100%,68%)
        /*216 */          LightSalmon1,//#ffaf87	rgb(255,175,135)	hsl(20,100%,76%)
        /*217 */          LightPink1,//#ffafaf	rgb(255,175,175)	hsl(0,100%,84%)
        /*218 */          Pink1,//#ffafd7	rgb(255,175,215)	hsl(330,100%,84%)
        /*219 */          Plum1,//#ffafff	rgb(255,175,255)	hsl(300,100%,84%)
        /*220 */          Gold1,//#ffd700	rgb(255,215,0)	hsl(0,100%,50%)
        /*221 */          LightGoldenrod4,//#ffd75f	rgb(255,215,95)	hsl(45,100%,68%)
        /*222 */          LightGoldenrod5,//#ffd787	rgb(255,215,135)	hsl(40,100%,76%)
        /*223 */          NavajoWhite1,//#ffd7af	rgb(255,215,175)	hsl(30,100%,84%)
        /*224 */          MistyRose1,//#ffd7d7	rgb(255,215,215)	hsl(0,100%,92%)
        /*225 */          Thistle1,//#ffd7ff	rgb(255,215,255)	hsl(300,100%,92%)
        /*226 */          Yellow1,//#ffff00	rgb(255,255,0)	hsl(60,100%,50%)
        /*227 */          LightGoldenrod1,//#ffff5f	rgb(255,255,95)	hsl(60,100%,68%)
        /*228 */          Khaki1,//#ffff87	rgb(255,255,135)	hsl(60,100%,76%)
        /*229 */          Wheat1,//#ffffaf	rgb(255,255,175)	hsl(60,100%,84%)
        /*230 */          Cornsilk1,//#ffffd7	rgb(255,255,215)	hsl(60,100%,92%)
        /*231 */          Grey100,//#ffffff	rgb(255,255,255)	hsl(0,0%,100%)
        /*232 */          Grey3,//#080808	rgb(8,8,8)	hsl(0,0%,3%)
        /*233 */          Grey7,//#121212	rgb(18,18,18)	hsl(0,0%,7%)
        /*234 */          Grey11,//#1c1c1c	rgb(28,28,28)	hsl(0,0%,10%)
        /*235 */          Grey15,//#262626	rgb(38,38,38)
        /*236 */          Grey19,//#303030	rgb(48,48,48)	hsl(0,0%,18%)
        /*237 */          Grey23,//#3a3a3a	rgb(58,58,58)	hsl(0,0%,22%)
        /*238 */          Grey27,//#444444	rgb(68,68,68)	hsl(0,0%,26%)
        /*239 */          Grey30,//#4e4e4e	rgb(78,78,78)	hsl(0,0%,30%)
        /*240 */          Grey35,//#585858	rgb(88,88,88)	hsl(0,0%,34%)
        /*241 */          Grey39,//#626262	rgb(98,98,98)	hsl(0,0%,37%)
        /*242 */          Grey42,//#6c6c6c	rgb(108,108,108)	hsl(0,0%,40%)
        /*243 */          Grey46,//#767676	rgb(118,118,118)	hsl(0,0%,46%)
        /*244 */          Grey50,//#808080	rgb(128,128,128)	hsl(0,0%,50%)
        /*245 */          Grey54,//#8a8a8a	rgb(138,138,138)	hsl(0,0%,54%)
        /*246 */          Grey58,//#949494	rgb(148,148,148)	hsl(0,0%,58%)
        /*247 */          Grey62,//#9e9e9e	rgb(158,158,158)	hsl(0,0%,61%)
        /*248 */          Grey66,//#a8a8a8	rgb(168,168,168)	hsl(0,0%,65%)
        /*249 */          Grey70,//#b2b2b2	rgb(178,178,178)	hsl(0,0%,69%)
        /*250 */          Grey74,//#bcbcbc	rgb(188,188,188)	hsl(0,0%,73%)
        /*251 */          Grey78,//#c6c6c6	rgb(198,198,198)	hsl(0,0%,77%)
        /*252 */          Grey82,//#d0d0d0	rgb(208,208,208)	hsl(0,0%,81%)
        /*253 */          Grey85,//#dadada	rgb(218,218,218)	hsl(0,0%,85%)
        /*254 */          Grey89,//#e4e4e4	rgb(228,228,228)	hsl(0,0%,89%)
        /*255 */          Grey93,//#eeeeee	rgb(238,238,238)	hsl(0,0%,93%)
        /*Reset*/         Reset,
        /*out Of bounds*/ OOB
    };
    enum class format
    {
        ansi256,
        html,
        conio
    };
    //struct rgb { uint8_t r = 0, r = 0, b = 0; }; // pour plus tard ... string operator ( "#RRGGBB", "rgb(r,b,g)", ...)
    
    static std::string ansi(color::type color_name, color::type BG_OR_OOB = color::OOB);
    static std::string html(color::type color_name);
    static std::string hrgb(color::type color_name);
};


struct color_data
{
    vxio::color::type _enum;
    uint8_t r;
    uint8_t g;
    uint8_t b;
    std::string to_string() const ;
    //uint32_t rgb() const;
};


}
/*!
 * @brief reassembling io string streams and template types input and output utilities to std::string
 *
 * @author &copy; 2021, Serge Lussier (lussier.serge@gmail.com); oldlonecoder on github.com and youtube
 */


namespace vxio{
    struct winbuffer;
}

class VXIO_API iostr
{
    static std::string __nullstr__;
    std::string _d;///< Private/Encapsulated std::string instance.
    std::string::size_type _arg_position = 0; // Initialize Argument index position...
    static std::string _default_token_separators;
    uint8_t _decimal_precision = 4;
    //char* __buff = nullptr;
    friend class vxio::winbuffer;
private:
    // %[flags][width][.precision][Length]specifier
    struct format_t
    {
        uint8_t F = 0; // Flag ( - + . # 0 ) => if s='s' then '-' => justify right; '+' => justify left; '^' => justify center.
        uint8_t W = 0; // Width ( Length )
        uint8_t R = 0; // Custom flag set if this format requires floating point spec.
        uint8_t P = 6; // Precision (Same as  default).
        uint8_t L = 0; // Length modifier ( linenum,ll,h,hh )
        std::size_t len = 0; // Format Length.
        std::size_t position = 0; // arg index position within _d.
        char S = 0; // Effective characeter code specifier such as 'd'; 's'; 'f'; 'l'; 'p'...
        const char* C = nullptr;

        format_t(std::string& Str) : C(Str.c_str())
        {}
        std::string operator()();

    };
    using lambda_fn_t = std::function<std::string(const iostr::format_t& Fmt)>;
public:

    using list_t = std::vector<std::string>;
    using Iterator = list_t::iterator;
    using CIterator = list_t::const_iterator;


//    struct TEACC_CORE_DLL geometry
//    {
//        iostr* bmap = nullptr;
//        geom::vxy cxy;
//        geom::rectangle r;
//        geom::size sz;
//
//        geometry& gotoxy(int x, int y);
//        geometry& operator << (const geom::vxy& xy) { return gotoxy(xy.x, xy.y); }
//        void set_geometry(int w, int h);
//
//        geometry& operator ++();
//        geometry& operator ++(int);
//        geometry& operator --();
//        geometry& operator --(int);
//        geometry& tput(const std::string& txt);
//
//        void clear();
//        void release();
//        template<typename T> iostr::geometry& operator << (T v)
//        {
//            iostr str;
//            str << v;
//            return tput(str());
//        }
//
//        std::string details();
//        operator std::string();
//
//    };
//
//    friend class geometry;
public:
    struct VXIO_API word
    {
        std::string::const_iterator start;
        std::string::const_iterator E;
        std::string::const_iterator SE;

        std::string operator()();
        std::string operator*();

        using list_t = std::vector<iostr::word>;
        using iterator_t = list_t::iterator;
        [[maybe_unused]] std::string mark() const;

        int line = 1;
        int col = 1;
        std::size_t position = 0;
        void operator++();
        void operator++(int);

        std::string location();
    };

private:
    struct s_p_s
    {
        std::string::const_iterator start;
        std::string::const_iterator pos;
        std::string::const_iterator stop; /// ...

        int         line = 1;
        int         col = 1;
        uint64_t    index = 0;

        s_p_s() = default;
        ~s_p_s() = default;

        s_p_s(const std::string& Str);
        bool skip();
        bool end();
        bool operator++();
        bool operator++(int);
        void reset(const std::string& _str)
        {
            pos = start = _str.cbegin();
            line = col = 1;
            index = 0;
            stop = _str.cend();
        }
        s_p_s& operator>>(iostr::word& w);
        //BCE& operator = (const Word& w);

    } _cursor;

public:

    iostr();
    iostr(const iostr& Str);
    iostr(iostr&& Str) noexcept;
    iostr(const std::string& aStr);
    iostr(std::string&& aStr);
    iostr(const char* aStr);

    ~iostr();

    iostr& operator=(const iostr&) = default;

    iostr& operator=(std::string && aStr);
    iostr& operator=(iostr && aStr) noexcept;
    iostr& operator=(const char* aStr);

    iostr& operator+=(const iostr & aStr);
    iostr& operator+=(const std::string & aStr);
    iostr& operator+=(char c);

    iostr& operator+(const iostr & aStr);
    iostr& operator+(const std::string & aStr);
    iostr& operator+(char c);

    template<typename T> iostr& operator+=(const T & a)
    {
        std::ostringstream os;
        os.precision(_decimal_precision);
        os << a;
        _d.append(os.str());
        return *this;
    }

    template<typename T> iostr& operator+(const T & a)
    {
        std::ostringstream os;
        os.precision(_decimal_precision);
        os << a;
        _d.append(os.str());
        return *this;
    }

    bool operator==(const iostr & aStr) const;

    bool empty() const
    {
        return _d.empty();
    }
    operator bool() const
    {
        return !_d.empty();
    }
    operator std::string()
    {
        return _d;
    }
    std::string& str()
    {
        return _d;
    }
    std::string operator()() const
    {
        return _d;
    }

    // -- on peut maintenant commencer nos routines de manipulations et de traitements....
    static iostr::list_t to_stdlist(int argc, char** argv);
    iostr& operator<<(const iostr & aStr);
    iostr& operator<<(const char* aStr);
    iostr& operator<<(const std::string & aStr);
    iostr& operator<<(char c);
    iostr& operator<<(vxio::color::type c);
    

    bool skip_ws(std::string::iterator & pos);
    static bool skip_ws(const char* pos);
    iostr& operator>>(std::string & _arg);
    static std::string make_str(const char* B, const char* E);
    template<typename T> iostr& operator>>(T & _arg)
    {

/*           if constexpr (std::is_same<T, uint64_t&>::value || std::is_same<T, uint16_t&>::value || std::is_same<T, uint32_t&>::value || std::is_same<T, uint64_t&>::value || std::is_same<T, uint8_t&>::value || std::is_same<T, uint16_t&>::value || std::is_same<T, uint32_t&>::value || std::is_same<T, uint64_t&>::value)
        {
            std::string::size_type pos;
            if (((pos = _D.find("0x")) != std::string::npos) || ((pos = _D.find("0X")) != std::string::npos))
            {
                pos += 2;
                std::istringstream i(_D.c_str() + pos);
                i >> std::hex >> _arg;
                return *this;
            }
        }*/
        std::istringstream in(_d); //  When iostr was derived from std::string ... Oops!  std::istringstream in(*this);
        if constexpr (
            std::is_same<float,T>::value || std::is_same<double, T>::value || std::is_same<long double, T>::value ||
            std::is_same<int, T>::value || std::is_same<unsigned int, T>::value ||
            std::is_same<int8_t, T>::value || std::is_same<uint8_t, T>::value ||
            std::is_same<int16_t, T>::value || std::is_same<uint16_t, T>::value ||
            std::is_same<int32_t, T>::value || std::is_same<uint32_t, T>::value ||
            std::is_same<int64_t, T>::value || std::is_same<uint64_t, T>::value
            )
        {
            in.precision(_decimal_precision);
            _arg = 0;
        }
        T R;
        in >> R;
        _arg = R;
        return *this;
    }

    char* duplicate() const
    {
        return strdup(_d.c_str());
    }


    const char* c_str()
    {
        return _d.c_str();
    }

//    struct TEACC_CORE_DLL rect_xy
//    {
//        int x = -1;
//        int y = -1;
//    };
//
//

    void clear();

    //virtual const std::string& tea_id() { return "iostr";}

    static std::string datetime(const std::string & str_fmt);

    template<typename t> iostr& operator=(const t & _a)
    {
        std::ostringstream os;
        os.precision(_decimal_precision);
        os << _a;
        clear();
        _d = os.str();
        return *this;
    }
    std::string extract_surrounded(const std::string & first_lhs, const std::string & first_rhs);
    [[nodiscard]] std::string::const_iterator scan_to(std::string::const_iterator start, char c) const;
    const char* scan_to(const char* start, char c) const;

    //bool test();
    iostr& octal(uint64_t __arg)
    {

        std::ostringstream os;
        os << std::oct << __arg;
        if (scan_arg() == std::string::npos)
        {
            _d.append(os.str());
            return *this;
        }

        return format<std::string >(os.str());
    }

    [[nodiscard]] size_t length() const
    {
        return _d.size();
    }
    char& operator[](size_t p)
    {
        return _d[p];
    }
    //bool empty() { return _str.Empty(); }

    static std::string default_token_separators()
    {
        return iostr::_default_token_separators;
    }
    static std::string token_separators()
    {
        return iostr::_default_token_separators;
    }
    std::size_t words(iostr::word::list_t & wcollection, const std::string & a_delimiters = "", bool keep_as_word = true) const;

    template<typename T> iostr& arg(T _arg)
    {
        if (scan_arg() == std::string::npos)
        {
            std::ostringstream os;
            os << std::oct << _arg;
            _d.append(os.str());
            return *this;
        }

        return format<T >(_arg);
    }
    int filter(const iostr::list_t & a_exp);

    template<typename T> std::string expand(const T & cnt)
    {
        iostr ss;

        int x = cnt.size();
        for (auto item : cnt)
        {
            ss << item;
            if (x-- > 1)
                ss << ',';
        }
        return ss();
    }

    static std::string upper_case(std::string s)
    {
        std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) {
            return std::toupper(c);
        });

        return s;
    }
    static std::string lower_case(std::string s)
    {
        std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) {
            return std::tolower(c);
        });

        return s;
    }

    static std::string SizeF(uint64_t sz);

    template<typename T> iostr& format(const T & _argv);
    /// <summary>
    /// Prepare removing std::sprintf calls.
    /// </summary>
    /// <typeparam name="T"></typeparam>
    /// <param name="fmt_data"></param>
    /// <param name="a_arg"></param>
    /// <returns>Reference to self</returns>
    template<typename T> iostr& input_arg(const iostr::format_t& fmt_data, const T& a_arg)
    {
        std::ostringstream os;
        os << a_arg;
        _d.erase(fmt_data.position, fmt_data.len);
        _d.insert(fmt_data.position, os.str());//  += os.str();
        return *this;
    }

    template<typename T> iostr& operator<<(const T & Argument)
    {
        if (scan_arg() == std::string::npos)
        {
            std::ostringstream os;
            os << Argument;
            _d += os.str();
            return *this;
        }

/*
         process_arg([Argument](const iostr::format_t& Fmt) -> std::string {
            if (Fmt.S == 'b')
            {
                bool pad = Fmt.F == '0';
                ///@todo Still have to resolve T here...
                if constexpr ((!std::is_same<T, const std::string&>::value) && (!std::is_same<T, const char*>::value))
                    return iostr::to_binary<T>(Argument, pad, Fmt.W <= 128 ? Fmt.W : 128);
            }
            if constexpr ((std::is_same<T, const std::string&>::value) || (std::is_same<T, const char*>::value))
                return Argument;

            std::ostringstream os;
            os << Argument;
            return os.str();
        });
*/
        return format(Argument);
    }

    template<typename T> iostr& hexadecimal(T & v)
    {
        std::stringstream is(_d.c_str() + 2);
        //std::cerr << " this:'" << _D.c_str()+2 << "' -> ";
        is >> std::hex >> v;
        //std::cerr << v << '\n';
        return *this;
    }

    static std::string type_of(std::string && func_desc);


    /*!
     * @brief "Stringify Bytes into binary representation.
     * @tparam T  Argument of type T
     * @param __arg Argument value.
     * @param padd Apdding value (number of padding 0's to fille the numberic base size in digits)
     * @param f  number_t of contiguous bits to group.
     * @return std::string
     */
    template<typename T> static std::string to_binary(T __arg, bool padd = false, int f = 128)
    {
        uint8_t seq;
        int nbytes = sizeof(T);

        uint8_t tableau[sizeof(T)];
        memcpy(tableau, (uint8_t*)&__arg, nbytes);
        std::string stream = "";
        int s = 0;
        //bool discard = false;
        for (int x = 1; x <= nbytes; x++)
        {
            seq = tableau[nbytes - x];
            if ((x == 1 && !padd && !seq) || (stream.empty() && !padd && !seq))
                continue;
            for (int y = 7; y >= 0; y--)
            { // est-ce que le bit #y est � 1 ?
                if (s >= f)
                {
                    stream += ' ';
                    s = 0;
                }
                ++s;
                uint8_t b = 1 << y;
                if (b & seq)
                    stream += '1';
                else
                    stream += '0';
            }
        }
        /*tableau.Clear();*/
        return stream;
    }
    /*!
        Important: renommer begin et end &agrave; cbegin et cend pour std::string::const_iterator !!
    */
    std::string::const_iterator begin() { return _d.begin(); }
    std::string::const_iterator end() { return _d.end(); }
    // --------------------------
private:
    std::string::size_type scan_arg();
    void process_arg(iostr::lambda_fn_t Fn);

    void put_arg(const std::string & aStr);
    int push_word(word::list_t & strm, word & w, std::string::size_type sz);

};

template<typename T> iostr& iostr::format(const T& _argv)
{
    format_t fmt = { _d };
    char buf[256];
    std::memset(buf, 0, 200);
    //LFnl << "\n";

    // Commentaires &eacute;crits en anglais seulement pour simplifier le texte.
    std::string::iterator c = _d.begin() + _arg_position;
    std::string::iterator n, beg, l;
    beg = n = c;
    ++c;
    // %[flag] :

    switch (*c)
    {
        case '-':
        case '+':
        case '#':
        case '0':fmt.F = *c++;
            break;
        default:
            //++m;
            break;
    }

    n = c;
    // %[width]:
    while ((n != _d.end()) && isdigit(*n))
        ++n;
    l = n; // save  ahead 'cursor position'
    --n;
    if (n >= c)
    {
        int t = 0;
        while (n >= c)
        // interpret format width value:
            fmt.W = fmt.W + (*(n--) - static_cast<uint64_t>('0')) * pow(10, t++);
    }
    else
        fmt.W = 0; // no width
    c = l; // update effective cursor position

    // check floating point format: m,n,l:=>  read positions
    if (*c == '.')
    {
        fmt.R = fmt.P;
        ++c;
        n = c;
        while ((n != _d.end()) && isdigit(*n)) ++n;
        l = n;
        --n;
        int t = 0;
        fmt.R = 0;
        while (n >= c)
            fmt.R = fmt.R + (*(n--) - static_cast<uint64_t>('0')) * pow(10, t++);
        c = l;
    }
    else
        fmt.R = fmt.P; // no floating point format

    //[.precision]
    n = c;
    //% ([Length]) [specifier]
    // format prefix ends;
    // Now the format type mnemonic:
    //  - remove the format string.
    //  - insert 'formatted/interpreted' value substring at the _arg_position:
    //  - reset _arg_position
    switch (*c)
    {
        case 'b':
        {
            if constexpr (!std::is_same<T, const std::string&>::value)
            {
                std::string BinaryStr;
                bool pad = fmt.F == '0';
                BinaryStr = iostr::to_binary<T>(_argv, pad, fmt.W <= 128 ? fmt.W : 128); // Limit grouping digits to 128 ...

                //std::sprintf(buf, "%s", BinaryStr.c_str());
                fmt.len = (c + 1) - beg;  //save format substring's length
                _d.erase(_arg_position, fmt.len);
                _d.insert(_arg_position, BinaryStr.c_str(), BinaryStr.length());
                _arg_position = 0;
                return *this;
            }
            break;
        }

        case 'd': // Decimale ou entier
        case 'i':fmt.S = *c++;
            break;
        case 'x':
        case 'X':fmt.S = *c++;
            break;
        case 'f':
        case 'F':
        case 'g':
        case 'G':fmt.S = *c++;
            break;
        case 's':
        case 'c':fmt.S = *c++;
            break;
        default:
            break;
    }

    fmt.len = c - beg;
    //std::cout << __PRETTY_FUNCTION__ << '\n' << __LINE__ << " _D:\n'" << _D << "':\n";
    std::string ff(_d, _arg_position, fmt.len);
    //std::cout << "ff(_d, _arg_position, fmt.len): '" << ff << "'\n";
    // -- Clang-tidy:error: cannot pass object of non-trivial type 'const std::__cxx11::basic_string<char>' through variadic function
    //
    if constexpr (std::is_same<T, const std::string&>::value)
    {
        std::snprintf(buf, 199, ff.c_str(), _argv.c_str());
        //std::cout << "\nbuffer[argv=const std::string&]:'" << buf << "'\n";
    }
    else
    if constexpr (std::is_same<T, std::string>::value)
    {
        std::snprintf(buf, 199, ff.c_str(), _argv.c_str());
        //std::cout << "\nbuffer[argv=std::string]:'" << buf << "'\n";
    }
    else
    {
        std::sprintf(buf, ff.c_str(), _argv);
        //std::cout << "\nbuffer[argv=T (const T&)]:'" << buf << "'\n";
    }
    _d.erase(_arg_position, fmt.len);
    _d.insert(_arg_position, buf, std::strlen(buf));
    _arg_position = 0;
    return *this;
};





//#endif //VXIO_FRAMEWORK_IOSTR_H
