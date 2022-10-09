/**********************************************************************************
*\file         MathUtils.h
*\brief        MathUtils.h Script
*
*\author	   Lee Jian Yan, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
					 or disclosure of this file or its contents without the prior
					 written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#ifndef MATH_UTILS_H
#define MATH_UTILS_H

#include <cmath>
#include <map>

#define EPSILON 0.00001f
#define PIf 3.1415926535897932384626433832795f

//----- Trigo list
static std::map<int, float> sinList{ {-360, 0.f}, {-359, 0.0174526f}, {-358, 0.0348996f}, {-357, 0.052336f}, {-356, 0.0697564f},
									{-355, 0.087156f}, {-354, 0.104529f}, {-353, 0.121869f}, {-352, 0.139173f}, {-351, 0.156434f},
									{-350, 0.173648f}, {-349, 0.190809f}, {-348, 0.207912f}, {-347, 0.224951f}, {-346, 0.241922f},
									{-345, 0.258819f}, {-344, 0.275638f}, {-343, 0.292372f}, {-342, 0.309017f}, {-341, 0.325568f},
									{-340, 0.34202f}, {-339, 0.358368f}, {-338, 0.374607f}, {-337, 0.390731f}, {-336, 0.406736f},
									{-335, 0.422618f}, {-334, 0.438371f}, {-333, 0.453991f}, {-332, 0.469471f}, {-331, 0.484809f},
									{-330, 0.5f}, {-329, 0.515038f}, {-328, 0.529919f}, {-327, 0.544639f}, {-326, 0.559193f},
									{-325, 0.573577f}, {-324, 0.587785f}, {-323, 0.601815f}, {-322, 0.615661f}, {-321, 0.629321f},
									{-320, 0.642788f}, {-319, 0.656059f}, {-318, 0.669131f}, {-317, 0.681998f}, {-316, 0.694659f},
									{-315, 0.707107f}, {-314, 0.71934f}, {-313, 0.731354f}, {-312, 0.743145f}, {-311, 0.75471f},
									{-310, 0.766045f}, {-309, 0.777146f}, {-308, 0.788011f}, {-307, 0.798635f}, {-306, 0.809017f},
									{-305, 0.819152f}, {-304, 0.829038f}, {-303, 0.838671f}, {-302, 0.848048f}, {-301, 0.857167f},
									{-300, 0.866025f}, {-299, 0.87462f}, {-298, 0.882948f}, {-297, 0.891007f}, {-296, 0.898794f},
									{-295, 0.906308f}, {-294, 0.913545f}, {-293, 0.920505f}, {-292, 0.927184f}, {-291, 0.93358f},
									{-290, 0.939693f}, {-289, 0.945519f}, {-288, 0.951056f}, {-287, 0.956305f}, {-286, 0.961262f},
									{-285, 0.965926f}, {-284, 0.970296f}, {-283, 0.97437f}, {-282, 0.978148f}, {-281, 0.981627f},
									{-280, 0.984808f}, {-279, 0.987688f}, {-278, 0.990268f}, {-277, 0.992546f}, {-276, 0.994522f},
									{-275, 0.996195f}, {-274, 0.997564f}, {-273, 0.99863f}, {-272, 0.999391f}, {-271, 0.999848f},
									{-270, 1.f}, {-269, 0.999848f}, {-268, 0.999391f}, {-267, 0.99863f}, {-266, 0.997564f},
									{-265, 0.996195f}, {-264, 0.994522f}, {-263, 0.992546f}, {-262, 0.990268f}, {-261, 0.987688f},
									{-260, 0.984808f}, {-259, 0.981627f}, {-258, 0.978148f}, {-257, 0.97437f}, {-256, 0.970296f},
									{-255, 0.965926f}, {-254, 0.961262f}, {-253, 0.956305f}, {-252, 0.951056f}, {-251, 0.945519f},
									{-250, 0.939693f}, {-249, 0.93358f}, {-248, 0.927184f}, {-247, 0.920505f}, {-246, 0.913545f},
									{-245, 0.906308f}, {-244, 0.898794f}, {-243, 0.891006f}, {-242, 0.882948f}, {-241, 0.87462f},
									{-240, 0.866025f}, {-239, 0.857167f}, {-238, 0.848048f}, {-237, 0.838671f}, {-236, 0.829038f},
									{-235, 0.819152f}, {-234, 0.809017f}, {-233, 0.798635f}, {-232, 0.788011f}, {-231, 0.777146f},
									{-230, 0.766045f}, {-229, 0.75471f}, {-228, 0.743145f}, {-227, 0.731354f}, {-226, 0.71934f},
									{-225, 0.707107f}, {-224, 0.694658f}, {-223, 0.681998f}, {-222, 0.669131f}, {-221, 0.656059f},
									{-220, 0.642788f}, {-219, 0.62932f}, {-218, 0.615661f}, {-217, 0.601815f}, {-216, 0.587785f},
									{-215, 0.573576f}, {-214, 0.559193f}, {-213, 0.544639f}, {-212, 0.529919f}, {-211, 0.515038f},
									{-210, 0.5f}, {-209, 0.48481f}, {-208, 0.469472f}, {-207, 0.453991f}, {-206, 0.438371f},
									{-205, 0.422618f}, {-204, 0.406737f}, {-203, 0.390731f}, {-202, 0.374607f}, {-201, 0.358368f},
									{-200, 0.34202f}, {-199, 0.325568f}, {-198, 0.309017f}, {-197, 0.292372f}, {-196, 0.275637f},
									{-195, 0.258819f}, {-194, 0.241922f}, {-193, 0.224951f}, {-192, 0.207912f}, {-191, 0.190809f},
									{-190, 0.173648f}, {-189, 0.156434f}, {-188, 0.139173f}, {-187, 0.121869f}, {-186, 0.104528f},
									{-185, 0.0871558f}, {-184, 0.0697564f}, {-183, 0.052336f}, {-182, 0.0348994f}, {-181, 0.0174524f},
									{-180, 0.f}, {-179, -0.0174525f}, {-178, -0.0348995f}, {-177, -0.052336f}, {-176, -0.0697565f},
									{-175, -0.0871559f}, {-174, -0.104528f}, {-173, -0.121869f}, {-172, -0.139173f}, {-171, -0.156434f},
									{-170, -0.173648f}, {-169, -0.190809f}, {-168, -0.207912f}, {-167, -0.224951f}, {-166, -0.241922f},
									{-165, -0.258819f}, {-164, -0.275637f}, {-163, -0.292372f}, {-162, -0.309017f}, {-161, -0.325568f},
									{-160, -0.34202f}, {-159, -0.358368f}, {-158, -0.374607f}, {-157, -0.390731f}, {-156, -0.406737f},
									{-155, -0.422618f}, {-154, -0.438371f}, {-153, -0.453991f}, {-152, -0.469472f}, {-151, -0.48481f},
									{-150, -0.5f}, {-149, -0.515038f}, {-148, -0.529919f}, {-147, -0.544639f}, {-146, -0.559193f},
									{-145, -0.573576f}, {-144, -0.587785f}, {-143, -0.601815f}, {-142, -0.615661f}, {-141, -0.62932f},
									{-140, -0.642788f}, {-139, -0.656059f}, {-138, -0.669131f}, {-137, -0.681998f}, {-136, -0.694658f},
									{-135, -0.707107f}, {-134, -0.71934f}, {-133, -0.731354f}, {-132, -0.743145f}, {-131, -0.75471f},
									{-130, -0.766044f}, {-129, -0.777146f}, {-128, -0.788011f}, {-127, -0.798635f}, {-126, -0.809017f},
									{-125, -0.819152f}, {-124, -0.829038f}, {-123, -0.838671f}, {-122, -0.848048f}, {-121, -0.857167f},
									{-120, -0.866025f}, {-119, -0.87462f}, {-118, -0.882948f}, {-117, -0.891007f}, {-116, -0.898794f},
									{-115, -0.906308f}, {-114, -0.913545f}, {-113, -0.920505f}, {-112, -0.927184f}, {-111, -0.93358f},
									{-110, -0.939693f}, {-109, -0.945519f}, {-108, -0.951057f}, {-107, -0.956305f}, {-106, -0.961262f},
									{-105, -0.965926f}, {-104, -0.970296f}, {-103, -0.97437f}, {-102, -0.978148f}, {-101, -0.981627f},
									{-100, -0.984808f}, {-99, -0.987688f}, {-98, -0.990268f}, {-97, -0.992546f}, {-96, -0.994522f},
									{-95, -0.996195f}, {-94, -0.997564f}, {-93, -0.99863f}, {-92, -0.999391f}, {-91, -0.999848f},
									{-90, -1.f}, {-89, -0.999848f}, {-88, -0.999391f}, {-87, -0.99863f}, {-86, -0.997564f},
									{-85, -0.996195f}, {-84, -0.994522f}, {-83, -0.992546f}, {-82, -0.990268f}, {-81, -0.987688f},
									{-80, -0.984808f}, {-79, -0.981627f}, {-78, -0.978148f}, {-77, -0.97437f}, {-76, -0.970296f},
									{-75, -0.965926f}, {-74, -0.961262f}, {-73, -0.956305f}, {-72, -0.951057f}, {-71, -0.945519f},
									{-70, -0.939693f}, {-69, -0.93358f}, {-68, -0.927184f}, {-67, -0.920505f}, {-66, -0.913545f},
									{-65, -0.906308f}, {-64, -0.898794f}, {-63, -0.891007f}, {-62, -0.882948f}, {-61, -0.87462f},
									{-60, -0.866025f}, {-59, -0.857167f}, {-58, -0.848048f}, {-57, -0.838671f}, {-56, -0.829038f},
									{-55, -0.819152f}, {-54, -0.809017f}, {-53, -0.798636f}, {-52, -0.788011f}, {-51, -0.777146f},
									{-50, -0.766044f}, {-49, -0.75471f}, {-48, -0.743145f}, {-47, -0.731354f}, {-46, -0.71934f},
									{-45, -0.707107f}, {-44, -0.694658f}, {-43, -0.681998f}, {-42, -0.669131f}, {-41, -0.656059f},
									{-40, -0.642788f}, {-39, -0.62932f}, {-38, -0.615661f}, {-37, -0.601815f}, {-36, -0.587785f},
									{-35, -0.573576f}, {-34, -0.559193f}, {-33, -0.544639f}, {-32, -0.529919f}, {-31, -0.515038f},
									{-30, -0.5f}, {-29, -0.48481f}, {-28, -0.469472f}, {-27, -0.45399f}, {-26, -0.438371f},
									{-25, -0.422618f}, {-24, -0.406737f}, {-23, -0.390731f}, {-22, -0.374607f}, {-21, -0.358368f},
									{-20, -0.34202f}, {-19, -0.325568f}, {-18, -0.309017f}, {-17, -0.292372f}, {-16, -0.275637f},
									{-15, -0.258819f}, {-14, -0.241922f}, {-13, -0.224951f}, {-12, -0.207912f}, {-11, -0.190809f},
									{-10, -0.173648f}, {-9, -0.156434f}, {-8, -0.139173f}, {-7, -0.121869f}, {-6, -0.104528f},
									{-5, -0.0871557f}, {-4, -0.0697565f}, {-3, -0.052336f}, {-2, -0.0348995f}, {-1, -0.0174524f},
									{0, 0.f}, {1, 0.0174524f}, {2, 0.0348995f}, {3, 0.052336f}, {4, 0.0697565f},
									{5, 0.0871557f}, {6, 0.104528f}, {7, 0.121869f}, {8, 0.139173f}, {9, 0.156434f},
									{10, 0.173648f}, {11, 0.190809f}, {12, 0.207912f}, {13, 0.224951f}, {14, 0.241922f},
									{15, 0.258819f}, {16, 0.275637f}, {17, 0.292372f}, {18, 0.309017f}, {19, 0.325568f},
									{20, 0.34202f}, {21, 0.358368f}, {22, 0.374607f}, {23, 0.390731f}, {24, 0.406737f},
									{25, 0.422618f}, {26, 0.438371f}, {27, 0.45399f}, {28, 0.469472f}, {29, 0.48481f},
									{30, 0.5f}, {31, 0.515038f}, {32, 0.529919f}, {33, 0.544639f}, {34, 0.559193f},
									{35, 0.573576f}, {36, 0.587785f}, {37, 0.601815f}, {38, 0.615661f}, {39, 0.62932f},
									{40, 0.642788f}, {41, 0.656059f}, {42, 0.669131f}, {43, 0.681998f}, {44, 0.694658f},
									{45, 0.707107f}, {46, 0.71934f}, {47, 0.731354f}, {48, 0.743145f}, {49, 0.75471f},
									{50, 0.766044f}, {51, 0.777146f}, {52, 0.788011f}, {53, 0.798636f}, {54, 0.809017f},
									{55, 0.819152f}, {56, 0.829038f}, {57, 0.838671f}, {58, 0.848048f}, {59, 0.857167f},
									{60, 0.866025f}, {61, 0.87462f}, {62, 0.882948f}, {63, 0.891007f}, {64, 0.898794f},
									{65, 0.906308f}, {66, 0.913545f}, {67, 0.920505f}, {68, 0.927184f}, {69, 0.93358f},
									{70, 0.939693f}, {71, 0.945519f}, {72, 0.951057f}, {73, 0.956305f}, {74, 0.961262f},
									{75, 0.965926f}, {76, 0.970296f}, {77, 0.97437f}, {78, 0.978148f}, {79, 0.981627f},
									{80, 0.984808f}, {81, 0.987688f}, {82, 0.990268f}, {83, 0.992546f}, {84, 0.994522f},
									{85, 0.996195f}, {86, 0.997564f}, {87, 0.99863f}, {88, 0.999391f}, {89, 0.999848f},
									{90, 1.f}, {91, 0.999848f}, {92, 0.999391f}, {93, 0.99863f}, {94, 0.997564f},
									{95, 0.996195f}, {96, 0.994522f}, {97, 0.992546f}, {98, 0.990268f}, {99, 0.987688f},
									{100, 0.984808f}, {101, 0.981627f}, {102, 0.978148f}, {103, 0.97437f}, {104, 0.970296f},
									{105, 0.965926f}, {106, 0.961262f}, {107, 0.956305f}, {108, 0.951057f}, {109, 0.945519f},
									{110, 0.939693f}, {111, 0.93358f}, {112, 0.927184f}, {113, 0.920505f}, {114, 0.913545f},
									{115, 0.906308f}, {116, 0.898794f}, {117, 0.891007f}, {118, 0.882948f}, {119, 0.87462f},
									{120, 0.866025f}, {121, 0.857167f}, {122, 0.848048f}, {123, 0.838671f}, {124, 0.829038f},
									{125, 0.819152f}, {126, 0.809017f}, {127, 0.798635f}, {128, 0.788011f}, {129, 0.777146f},
									{130, 0.766044f}, {131, 0.75471f}, {132, 0.743145f}, {133, 0.731354f}, {134, 0.71934f},
									{135, 0.707107f}, {136, 0.694658f}, {137, 0.681998f}, {138, 0.669131f}, {139, 0.656059f},
									{140, 0.642788f}, {141, 0.62932f}, {142, 0.615661f}, {143, 0.601815f}, {144, 0.587785f},
									{145, 0.573576f}, {146, 0.559193f}, {147, 0.544639f}, {148, 0.529919f}, {149, 0.515038f},
									{150, 0.5f}, {151, 0.48481f}, {152, 0.469472f}, {153, 0.453991f}, {154, 0.438371f},
									{155, 0.422618f}, {156, 0.406737f}, {157, 0.390731f}, {158, 0.374607f}, {159, 0.358368f},
									{160, 0.34202f}, {161, 0.325568f}, {162, 0.309017f}, {163, 0.292372f}, {164, 0.275637f},
									{165, 0.258819f}, {166, 0.241922f}, {167, 0.224951f}, {168, 0.207912f}, {169, 0.190809f},
									{170, 0.173648f}, {171, 0.156434f}, {172, 0.139173f}, {173, 0.121869f}, {174, 0.104528f},
									{175, 0.0871559f}, {176, 0.0697565f}, {177, 0.052336f}, {178, 0.0348995f}, {179, 0.0174525f},
									{180, 0.f}, {181, -0.0174524f}, {182, -0.0348994f}, {183, -0.052336f}, {184, -0.0697564f},
									{185, -0.0871558f}, {186, -0.104528f}, {187, -0.121869f}, {188, -0.139173f}, {189, -0.156434f},
									{190, -0.173648f}, {191, -0.190809f}, {192, -0.207912f}, {193, -0.224951f}, {194, -0.241922f},
									{195, -0.258819f}, {196, -0.275637f}, {197, -0.292372f}, {198, -0.309017f}, {199, -0.325568f},
									{200, -0.34202f}, {201, -0.358368f}, {202, -0.374607f}, {203, -0.390731f}, {204, -0.406737f},
									{205, -0.422618f}, {206, -0.438371f}, {207, -0.453991f}, {208, -0.469472f}, {209, -0.48481f},
									{210, -0.5f}, {211, -0.515038f}, {212, -0.529919f}, {213, -0.544639f}, {214, -0.559193f},
									{215, -0.573576f}, {216, -0.587785f}, {217, -0.601815f}, {218, -0.615661f}, {219, -0.62932f},
									{220, -0.642788f}, {221, -0.656059f}, {222, -0.669131f}, {223, -0.681998f}, {224, -0.694658f},
									{225, -0.707107f}, {226, -0.71934f}, {227, -0.731354f}, {228, -0.743145f}, {229, -0.75471f},
									{230, -0.766045f}, {231, -0.777146f}, {232, -0.788011f}, {233, -0.798635f}, {234, -0.809017f},
									{235, -0.819152f}, {236, -0.829038f}, {237, -0.838671f}, {238, -0.848048f}, {239, -0.857167f},
									{240, -0.866025f}, {241, -0.87462f}, {242, -0.882948f}, {243, -0.891006f}, {244, -0.898794f},
									{245, -0.906308f}, {246, -0.913545f}, {247, -0.920505f}, {248, -0.927184f}, {249, -0.93358f},
									{250, -0.939693f}, {251, -0.945519f}, {252, -0.951056f}, {253, -0.956305f}, {254, -0.961262f},
									{255, -0.965926f}, {256, -0.970296f}, {257, -0.97437f}, {258, -0.978148f}, {259, -0.981627f},
									{260, -0.984808f}, {261, -0.987688f}, {262, -0.990268f}, {263, -0.992546f}, {264, -0.994522f},
									{265, -0.996195f}, {266, -0.997564f}, {267, -0.99863f}, {268, -0.999391f}, {269, -0.999848f},
									{270, -1.f}, {271, -0.999848f}, {272, -0.999391f}, {273, -0.99863f}, {274, -0.997564f},
									{275, -0.996195f}, {276, -0.994522f}, {277, -0.992546f}, {278, -0.990268f}, {279, -0.987688f},
									{280, -0.984808f}, {281, -0.981627f}, {282, -0.978148f}, {283, -0.97437f}, {284, -0.970296f},
									{285, -0.965926f}, {286, -0.961262f}, {287, -0.956305f}, {288, -0.951056f}, {289, -0.945519f},
									{290, -0.939693f}, {291, -0.93358f}, {292, -0.927184f}, {293, -0.920505f}, {294, -0.913545f},
									{295, -0.906308f}, {296, -0.898794f}, {297, -0.891007f}, {298, -0.882948f}, {299, -0.87462f},
									{300, -0.866025f}, {301, -0.857167f}, {302, -0.848048f}, {303, -0.838671f}, {304, -0.829038f},
									{305, -0.819152f}, {306, -0.809017f}, {307, -0.798635f}, {308, -0.788011f}, {309, -0.777146f},
									{310, -0.766045f}, {311, -0.75471f}, {312, -0.743145f}, {313, -0.731354f}, {314, -0.71934f},
									{315, -0.707107f}, {316, -0.694659f}, {317, -0.681998f}, {318, -0.669131f}, {319, -0.656059f},
									{320, -0.642788f}, {321, -0.629321f}, {322, -0.615661f}, {323, -0.601815f}, {324, -0.587785f},
									{325, -0.573577f}, {326, -0.559193f}, {327, -0.544639f}, {328, -0.529919f}, {329, -0.515038f},
									{330, -0.5f}, {331, -0.484809f}, {332, -0.469471f}, {333, -0.453991f}, {334, -0.438371f},
									{335, -0.422618f}, {336, -0.406736f}, {337, -0.390731f}, {338, -0.374607f}, {339, -0.358368f},
									{340, -0.34202f}, {341, -0.325568f}, {342, -0.309017f}, {343, -0.292372f}, {344, -0.275638f},
									{345, -0.258819f}, {346, -0.241922f}, {347, -0.224951f}, {348, -0.207912f}, {349, -0.190809f},
									{350, -0.173648f}, {351, -0.156434f}, {352, -0.139173f}, {353, -0.121869f}, {354, -0.104529f},
									{355, -0.087156f}, {356, -0.0697564f}, {357, -0.052336f}, {358, -0.0348996f}, {359, -0.0174526f},
									{ 360, 0.f } };

static std::map<int, float> cosList{ {-360, 1.f}, {-359, 0.999848f}, {-358, 0.999391f}, {-357, 0.99863f}, {-356, 0.997564f},
									{-355, 0.996195f}, {-354, 0.994522f}, {-353, 0.992546f}, {-352, 0.990268f}, {-351, 0.987688f},
									{-350, 0.984808f}, {-349, 0.981627f}, {-348, 0.978148f}, {-347, 0.97437f}, {-346, 0.970296f},
									{-345, 0.965926f}, {-344, 0.961262f}, {-343, 0.956305f}, {-342, 0.951057f}, {-341, 0.945519f},
									{-340, 0.939693f}, {-339, 0.93358f}, {-338, 0.927184f}, {-337, 0.920505f}, {-336, 0.913546f},
									{-335, 0.906308f}, {-334, 0.898794f}, {-333, 0.891007f}, {-332, 0.882948f}, {-331, 0.87462f},
									{-330, 0.866025f}, {-329, 0.857167f}, {-328, 0.848048f}, {-327, 0.838671f}, {-326, 0.829037f},
									{-325, 0.819152f}, {-324, 0.809017f}, {-323, 0.798636f}, {-322, 0.788011f}, {-321, 0.777146f},
									{-320, 0.766044f}, {-319, 0.75471f}, {-318, 0.743145f}, {-317, 0.731354f}, {-316, 0.71934f},
									{-315, 0.707107f}, {-314, 0.694658f}, {-313, 0.681998f}, {-312, 0.669131f}, {-311, 0.656059f},
									{-310, 0.642788f}, {-309, 0.62932f}, {-308, 0.615662f}, {-307, 0.601815f}, {-306, 0.587785f},
									{-305, 0.573576f}, {-304, 0.559193f}, {-303, 0.544639f}, {-302, 0.529919f}, {-301, 0.515038f},
									{-300, 0.5f}, {-299, 0.48481f}, {-298, 0.469472f}, {-297, 0.45399f}, {-296, 0.438371f},
									{-295, 0.422618f}, {-294, 0.406737f}, {-293, 0.390731f}, {-292, 0.374606f}, {-291, 0.358368f},
									{-290, 0.34202f}, {-289, 0.325568f}, {-288, 0.309017f}, {-287, 0.292371f}, {-286, 0.275637f},
									{-285, 0.258819f}, {-284, 0.241922f}, {-283, 0.224951f}, {-282, 0.207911f}, {-281, 0.190809f},
									{-280, 0.173648f}, {-279, 0.156435f}, {-278, 0.139173f}, {-277, 0.121869f}, {-276, 0.104528f},
									{-275, 0.0871557f}, {-274, 0.0697566f}, {-273, 0.0523361f}, {-272, 0.0348993f}, {-271, 0.0174523f},
									{-270, 0.f}, {-269, -0.0174523f}, {-268, -0.0348998f}, {-267, -0.0523361f}, {-266, -0.0697565f},
									{-265, -0.0871557f}, {-264, -0.104528f}, {-263, -0.12187f}, {-262, -0.139173f}, {-261, -0.156435f},
									{-260, -0.173648f}, {-259, -0.190809f}, {-258, -0.207912f}, {-257, -0.224951f}, {-256, -0.241922f},
									{-255, -0.258819f}, {-254, -0.275637f}, {-253, -0.292372f}, {-252, -0.309017f}, {-251, -0.325568f},
									{-250, -0.34202f}, {-249, -0.358368f}, {-248, -0.374607f}, {-247, -0.390731f}, {-246, -0.406737f},
									{-245, -0.422618f}, {-244, -0.438371f}, {-243, -0.453991f}, {-242, -0.469472f}, {-241, -0.48481f},
									{-240, -0.5f}, {-239, -0.515038f}, {-238, -0.529919f}, {-237, -0.544639f}, {-236, -0.559193f},
									{-235, -0.573576f}, {-234, -0.587785f}, {-233, -0.601815f}, {-232, -0.615662f}, {-231, -0.62932f},
									{-230, -0.642787f}, {-229, -0.656059f}, {-228, -0.669131f}, {-227, -0.681998f}, {-226, -0.694658f},
									{-225, -0.707107f}, {-224, -0.71934f}, {-223, -0.731354f}, {-222, -0.743145f}, {-221, -0.75471f},
									{-220, -0.766044f}, {-219, -0.777146f}, {-218, -0.788011f}, {-217, -0.798636f}, {-216, -0.809017f},
									{-215, -0.819152f}, {-214, -0.829038f}, {-213, -0.838671f}, {-212, -0.848048f}, {-211, -0.857167f},
									{-210, -0.866025f}, {-209, -0.87462f}, {-208, -0.882948f}, {-207, -0.891007f}, {-206, -0.898794f},
									{-205, -0.906308f}, {-204, -0.913546f}, {-203, -0.920505f}, {-202, -0.927184f}, {-201, -0.93358f},
									{-200, -0.939693f}, {-199, -0.945519f}, {-198, -0.951057f}, {-197, -0.956305f}, {-196, -0.961262f},
									{-195, -0.965926f}, {-194, -0.970296f}, {-193, -0.97437f}, {-192, -0.978148f}, {-191, -0.981627f},
									{-190, -0.984808f}, {-189, -0.987688f}, {-188, -0.990268f}, {-187, -0.992546f}, {-186, -0.994522f},
									{-185, -0.996195f}, {-184, -0.997564f}, {-183, -0.99863f}, {-182, -0.999391f}, {-181, -0.999848f},
									{-180, -1.f}, {-179, -0.999848f}, {-178, -0.999391f}, {-177, -0.99863f}, {-176, -0.997564f},
									{-175, -0.996195f}, {-174, -0.994522f}, {-173, -0.992546f}, {-172, -0.990268f}, {-171, -0.987688f},
									{-170, -0.984808f}, {-169, -0.981627f}, {-168, -0.978148f}, {-167, -0.97437f}, {-166, -0.970296f},
									{-165, -0.965926f}, {-164, -0.961262f}, {-163, -0.956305f}, {-162, -0.951057f}, {-161, -0.945519f},
									{-160, -0.939693f}, {-159, -0.93358f}, {-158, -0.927184f}, {-157, -0.920505f}, {-156, -0.913545f},
									{-155, -0.906308f}, {-154, -0.898794f}, {-153, -0.891006f}, {-152, -0.882948f}, {-151, -0.87462f},
									{-150, -0.866025f}, {-149, -0.857167f}, {-148, -0.848048f}, {-147, -0.838671f}, {-146, -0.829038f},
									{-145, -0.819152f}, {-144, -0.809017f}, {-143, -0.798635f}, {-142, -0.788011f}, {-141, -0.777146f},
									{-140, -0.766044f}, {-139, -0.75471f}, {-138, -0.743145f}, {-137, -0.731354f}, {-136, -0.71934f},
									{-135, -0.707107f}, {-134, -0.694658f}, {-133, -0.681998f}, {-132, -0.669131f}, {-131, -0.656059f},
									{-130, -0.642788f}, {-129, -0.62932f}, {-128, -0.615661f}, {-127, -0.601815f}, {-126, -0.587785f},
									{-125, -0.573576f}, {-124, -0.559193f}, {-123, -0.544639f}, {-122, -0.529919f}, {-121, -0.515038f},
									{-120, -0.5f}, {-119, -0.48481f}, {-118, -0.469472f}, {-117, -0.45399f}, {-116, -0.438371f},
									{-115, -0.422618f}, {-114, -0.406737f}, {-113, -0.390731f}, {-112, -0.374607f}, {-111, -0.358368f},
									{-110, -0.34202f}, {-109, -0.325568f}, {-108, -0.309017f}, {-107, -0.292372f}, {-106, -0.275637f},
									{-105, -0.258819f}, {-104, -0.241922f}, {-103, -0.224951f}, {-102, -0.207912f}, {-101, -0.190809f},
									{-100, -0.173648f}, {-99, -0.156434f}, {-98, -0.139173f}, {-97, -0.121869f}, {-96, -0.104529f},
									{-95, -0.0871558f}, {-94, -0.0697565f}, {-93, -0.0523359f}, {-92, -0.0348995f}, {-91, -0.0174524f},
									{-90, 0.f}, {-89, 0.0174524f}, {-88, 0.0348995f}, {-87, 0.052336f}, {-86, 0.0697565f},
									{-85, 0.0871558f}, {-84, 0.104528f}, {-83, 0.121869f}, {-82, 0.139173f}, {-81, 0.156434f},
									{-80, 0.173648f}, {-79, 0.190809f}, {-78, 0.207912f}, {-77, 0.224951f}, {-76, 0.241922f},
									{-75, 0.258819f}, {-74, 0.275637f}, {-73, 0.292372f}, {-72, 0.309017f}, {-71, 0.325568f},
									{-70, 0.34202f}, {-69, 0.358368f}, {-68, 0.374607f}, {-67, 0.390731f}, {-66, 0.406737f},
									{-65, 0.422618f}, {-64, 0.438371f}, {-63, 0.453991f}, {-62, 0.469472f}, {-61, 0.48481f},
									{-60, 0.5f}, {-59, 0.515038f}, {-58, 0.529919f}, {-57, 0.544639f}, {-56, 0.559193f},
									{-55, 0.573576f}, {-54, 0.587785f}, {-53, 0.601815f}, {-52, 0.615661f}, {-51, 0.62932f},
									{-50, 0.642788f}, {-49, 0.656059f}, {-48, 0.669131f}, {-47, 0.681998f}, {-46, 0.694658f},
									{-45, 0.707107f}, {-44, 0.71934f}, {-43, 0.731354f}, {-42, 0.743145f}, {-41, 0.75471f},
									{-40, 0.766044f}, {-39, 0.777146f}, {-38, 0.788011f}, {-37, 0.798636f}, {-36, 0.809017f},
									{-35, 0.819152f}, {-34, 0.829038f}, {-33, 0.838671f}, {-32, 0.848048f}, {-31, 0.857167f},
									{-30, 0.866025f}, {-29, 0.87462f}, {-28, 0.882948f}, {-27, 0.891007f}, {-26, 0.898794f},
									{-25, 0.906308f}, {-24, 0.913545f}, {-23, 0.920505f}, {-22, 0.927184f}, {-21, 0.93358f},
									{-20, 0.939693f}, {-19, 0.945519f}, {-18, 0.951057f}, {-17, 0.956305f}, {-16, 0.961262f},
									{-15, 0.965926f}, {-14, 0.970296f}, {-13, 0.97437f}, {-12, 0.978148f}, {-11, 0.981627f},
									{-10, 0.984808f}, {-9, 0.987688f}, {-8, 0.990268f}, {-7, 0.992546f}, {-6, 0.994522f},
									{-5, 0.996195f}, {-4, 0.997564f}, {-3, 0.99863f}, {-2, 0.999391f}, {-1, 0.999848f},
									{0, 1.f}, {1, 0.999848f}, {2, 0.999391f}, {3, 0.99863f}, {4, 0.997564f},
									{5, 0.996195f}, {6, 0.994522f}, {7, 0.992546f}, {8, 0.990268f}, {9, 0.987688f},
									{10, 0.984808f}, {11, 0.981627f}, {12, 0.978148f}, {13, 0.97437f}, {14, 0.970296f},
									{15, 0.965926f}, {16, 0.961262f}, {17, 0.956305f}, {18, 0.951057f}, {19, 0.945519f},
									{20, 0.939693f}, {21, 0.93358f}, {22, 0.927184f}, {23, 0.920505f}, {24, 0.913545f},
									{25, 0.906308f}, {26, 0.898794f}, {27, 0.891007f}, {28, 0.882948f}, {29, 0.87462f},
									{30, 0.866025f}, {31, 0.857167f}, {32, 0.848048f}, {33, 0.838671f}, {34, 0.829038f},
									{35, 0.819152f}, {36, 0.809017f}, {37, 0.798636f}, {38, 0.788011f}, {39, 0.777146f},
									{40, 0.766044f}, {41, 0.75471f}, {42, 0.743145f}, {43, 0.731354f}, {44, 0.71934f},
									{45, 0.707107f}, {46, 0.694658f}, {47, 0.681998f}, {48, 0.669131f}, {49, 0.656059f},
									{50, 0.642788f}, {51, 0.62932f}, {52, 0.615661f}, {53, 0.601815f}, {54, 0.587785f},
									{55, 0.573576f}, {56, 0.559193f}, {57, 0.544639f}, {58, 0.529919f}, {59, 0.515038f},
									{60, 0.5f}, {61, 0.48481f}, {62, 0.469472f}, {63, 0.453991f}, {64, 0.438371f},
									{65, 0.422618f}, {66, 0.406737f}, {67, 0.390731f}, {68, 0.374607f}, {69, 0.358368f},
									{70, 0.34202f}, {71, 0.325568f}, {72, 0.309017f}, {73, 0.292372f}, {74, 0.275637f},
									{75, 0.258819f}, {76, 0.241922f}, {77, 0.224951f}, {78, 0.207912f}, {79, 0.190809f},
									{80, 0.173648f}, {81, 0.156434f}, {82, 0.139173f}, {83, 0.121869f}, {84, 0.104528f},
									{85, 0.0871558f}, {86, 0.0697565f}, {87, 0.052336f}, {88, 0.0348995f}, {89, 0.0174524f},
									{90, 0.f}, {91, -0.0174524f}, {92, -0.0348995f}, {93, -0.0523359f}, {94, -0.0697565f},
									{95, -0.0871558f}, {96, -0.104529f}, {97, -0.121869f}, {98, -0.139173f}, {99, -0.156434f},
									{100, -0.173648f}, {101, -0.190809f}, {102, -0.207912f}, {103, -0.224951f}, {104, -0.241922f},
									{105, -0.258819f}, {106, -0.275637f}, {107, -0.292372f}, {108, -0.309017f}, {109, -0.325568f},
									{110, -0.34202f}, {111, -0.358368f}, {112, -0.374607f}, {113, -0.390731f}, {114, -0.406737f},
									{115, -0.422618f}, {116, -0.438371f}, {117, -0.45399f}, {118, -0.469472f}, {119, -0.48481f},
									{120, -0.5f}, {121, -0.515038f}, {122, -0.529919f}, {123, -0.544639f}, {124, -0.559193f},
									{125, -0.573576f}, {126, -0.587785f}, {127, -0.601815f}, {128, -0.615661f}, {129, -0.62932f},
									{130, -0.642788f}, {131, -0.656059f}, {132, -0.669131f}, {133, -0.681998f}, {134, -0.694658f},
									{135, -0.707107f}, {136, -0.71934f}, {137, -0.731354f}, {138, -0.743145f}, {139, -0.75471f},
									{140, -0.766044f}, {141, -0.777146f}, {142, -0.788011f}, {143, -0.798635f}, {144, -0.809017f},
									{145, -0.819152f}, {146, -0.829038f}, {147, -0.838671f}, {148, -0.848048f}, {149, -0.857167f},
									{150, -0.866025f}, {151, -0.87462f}, {152, -0.882948f}, {153, -0.891006f}, {154, -0.898794f},
									{155, -0.906308f}, {156, -0.913545f}, {157, -0.920505f}, {158, -0.927184f}, {159, -0.93358f},
									{160, -0.939693f}, {161, -0.945519f}, {162, -0.951057f}, {163, -0.956305f}, {164, -0.961262f},
									{165, -0.965926f}, {166, -0.970296f}, {167, -0.97437f}, {168, -0.978148f}, {169, -0.981627f},
									{170, -0.984808f}, {171, -0.987688f}, {172, -0.990268f}, {173, -0.992546f}, {174, -0.994522f},
									{175, -0.996195f}, {176, -0.997564f}, {177, -0.99863f}, {178, -0.999391f}, {179, -0.999848f},
									{180, -1.f}, {181, -0.999848f}, {182, -0.999391f}, {183, -0.99863f}, {184, -0.997564f},
									{185, -0.996195f}, {186, -0.994522f}, {187, -0.992546f}, {188, -0.990268f}, {189, -0.987688f},
									{190, -0.984808f}, {191, -0.981627f}, {192, -0.978148f}, {193, -0.97437f}, {194, -0.970296f},
									{195, -0.965926f}, {196, -0.961262f}, {197, -0.956305f}, {198, -0.951057f}, {199, -0.945519f},
									{200, -0.939693f}, {201, -0.93358f}, {202, -0.927184f}, {203, -0.920505f}, {204, -0.913546f},
									{205, -0.906308f}, {206, -0.898794f}, {207, -0.891007f}, {208, -0.882948f}, {209, -0.87462f},
									{210, -0.866025f}, {211, -0.857167f}, {212, -0.848048f}, {213, -0.838671f}, {214, -0.829038f},
									{215, -0.819152f}, {216, -0.809017f}, {217, -0.798636f}, {218, -0.788011f}, {219, -0.777146f},
									{220, -0.766044f}, {221, -0.75471f}, {222, -0.743145f}, {223, -0.731354f}, {224, -0.71934f},
									{225, -0.707107f}, {226, -0.694658f}, {227, -0.681998f}, {228, -0.669131f}, {229, -0.656059f},
									{230, -0.642787f}, {231, -0.62932f}, {232, -0.615662f}, {233, -0.601815f}, {234, -0.587785f},
									{235, -0.573576f}, {236, -0.559193f}, {237, -0.544639f}, {238, -0.529919f}, {239, -0.515038f},
									{240, -0.5f}, {241, -0.48481f}, {242, -0.469472f}, {243, -0.453991f}, {244, -0.438371f},
									{245, -0.422618f}, {246, -0.406737f}, {247, -0.390731f}, {248, -0.374607f}, {249, -0.358368f},
									{250, -0.34202f}, {251, -0.325568f}, {252, -0.309017f}, {253, -0.292372f}, {254, -0.275637f},
									{255, -0.258819f}, {256, -0.241922f}, {257, -0.224951f}, {258, -0.207912f}, {259, -0.190809f},
									{260, -0.173648f}, {261, -0.156435f}, {262, -0.139173f}, {263, -0.12187f}, {264, -0.104528f},
									{265, -0.0871557f}, {266, -0.0697565f}, {267, -0.0523361f}, {268, -0.0348998f}, {269, -0.0174523f},
									{270, 1.19249e-08f}, {271, 0.0174523f}, {272, 0.0348993f}, {273, 0.0523361f}, {274, 0.0697566f},
									{275, 0.0871557f}, {276, 0.104528f}, {277, 0.121869f}, {278, 0.139173f}, {279, 0.156435f},
									{280, 0.173648f}, {281, 0.190809f}, {282, 0.207911f}, {283, 0.224951f}, {284, 0.241922f},
									{285, 0.258819f}, {286, 0.275637f}, {287, 0.292371f}, {288, 0.309017f}, {289, 0.325568f},
									{290, 0.34202f}, {291, 0.358368f}, {292, 0.374606f}, {293, 0.390731f}, {294, 0.406737f},
									{295, 0.422618f}, {296, 0.438371f}, {297, 0.45399f}, {298, 0.469472f}, {299, 0.48481f},
									{300, 0.5f}, {301, 0.515038f}, {302, 0.529919f}, {303, 0.544639f}, {304, 0.559193f},
									{305, 0.573576f}, {306, 0.587785f}, {307, 0.601815f}, {308, 0.615662f}, {309, 0.62932f},
									{310, 0.642788f}, {311, 0.656059f}, {312, 0.669131f}, {313, 0.681998f}, {314, 0.694658f},
									{315, 0.707107f}, {316, 0.71934f}, {317, 0.731354f}, {318, 0.743145f}, {319, 0.75471f},
									{320, 0.766044f}, {321, 0.777146f}, {322, 0.788011f}, {323, 0.798636f}, {324, 0.809017f},
									{325, 0.819152f}, {326, 0.829037f}, {327, 0.838671f}, {328, 0.848048f}, {329, 0.857167f},
									{330, 0.866025f}, {331, 0.87462f}, {332, 0.882948f}, {333, 0.891007f}, {334, 0.898794f},
									{335, 0.906308f}, {336, 0.913546f}, {337, 0.920505f}, {338, 0.927184f}, {339, 0.93358f},
									{340, 0.939693f}, {341, 0.945519f}, {342, 0.951057f}, {343, 0.956305f}, {344, 0.961262f},
									{345, 0.965926f}, {346, 0.970296f}, {347, 0.97437f}, {348, 0.978148f}, {349, 0.981627f},
									{350, 0.984808f}, {351, 0.987688f}, {352, 0.990268f}, {353, 0.992546f}, {354, 0.994522f},
									{355, 0.996195f}, {356, 0.997564f}, {357, 0.99863f}, {358, 0.999391f}, {359, 0.999848f},
									{ 360, 1.f } };


namespace paperback
{
	//----- MathUtils static class definition
	//-----| Math helper class that provides 
	//-----| utility functions like radians / deg
	//-----| conversion, pow, float comparisons, etc.
	class MathUtils
	{
	public:
		//----------------------------------------------------------------------//
		// //----- Convert radian to degree
		static inline float RadToDeg(float Radian) {
			return Radian * (180 / PIf);
		}

		//----- Converts degrees to radians
		static inline float DegToRad(float Degree) {
			return Degree * (PIf / 180);
		}

		//----- Checks if 2 floats are equal with EPSILON value
		static inline bool IsEqualf(float a, float b) {
			return fabsf(a - b) <= EPSILON;
		}

		//----------------------------------------------------------------------//
		//----- Cotangent functions
		static double cot(double val) {
			return 1.0 / tan(val);
		}
		static float cotf(float val) {
			return 1.f / tan(val);
		}

		//----------------------------------------------------------------------//
		//----- Swaps two values
		template <typename T>
		static inline void Swap(T& a, T& b)	
		{
			T temp = a;
			a = b;
			b = temp;
		}

		//----------------------------------------------------------------------//
		//----- Min definition
		template <typename T>
		static T Min(T first, T second)	{
			return first < second ? first : second;
		}

		//----- Max definition
		template <typename T>
		static T Max(T first, T second)	{
			return first > second ? first : second;
		}

		//----------------------------------------------------------------------//
		//----- GetEven compile-time function
		//-----| Given a number, return the even equivalent
		//-----| E.g. GetEven<8>() = 8
		//-----|      GetEven<7>() = 8
		template <size_t N>
		static constexpr size_t GetEven() 
		{
			if constexpr (N % 2)
				return N + 1;
			else return N;
		}

		//----- GetOdd compile-time function
		//-----| Given a number, return the odd equivalent
		//-----| E.g. GetOdd<7>() = 7
		//-----|      GetOdd<6>() = 7
		template <size_t N>
		static constexpr size_t GetOdd()
		{
			if constexpr (N % 2)
				return N;
			else return N + 1;
		}

		//----- GetPositive compile-time function
		//-----| Given a number, return the positive
		//-----| value (absolute / mod).
		//-----| E.g. GetPositive<7>() = 7
		//-----|      GetPositive<-7>() = 7
		template <long long N>
		static constexpr long long GetPositive()
		{
			if constexpr (N < 0)
				return -N;
			else return N;
		}
		
		//----------------------------------------------------------------------//
		//----- Power meta programming
		//-----| E.g. Pow<5, 3>::val = 125 (5^3)
		template <size_t Base, size_t Power>
		struct Pow
		{
			static const size_t val = Base * Pow<Base, Power - 1>::val;
		};
		template <size_t Base>
		struct Pow <Base, 1>
		{
			static const size_t val = Base;
		};

		//----------------------------------------------------------------------//
		//----- Fast inverse sqrt : wikipedia
		static inline float Inv_sqrt(float value)
		{
#pragma warning(disable : 4201)
			union
			{
				float fpoint;
				size_t integer;
			} inverse;
#pragma warning(default : 4201)
			float second = value * 0.5f;
			const float third = 1.5F;
			inverse.fpoint = value;
			inverse.integer = 0x5f3759df - (inverse.integer >> 1);
			inverse.fpoint = inverse.fpoint * 
				(third - (second * inverse.fpoint * inverse.fpoint));
			return inverse.fpoint;
		}

		// only if needed for slight performance boost
		//----------------------------------------------------------------------//
		//----- This sin function uses the std::map 
		static float Sinf(float radians)
		{
			int degrees = static_cast<int>(RadToDeg(radians));
			if (degrees > 360 || degrees < -360) { degrees %= 360; }

			return sinList[degrees];
		}

		//----- This cos function uses the std::map 
		static float Cosf(float radians)
		{
			int degrees = static_cast<int>(RadToDeg(radians));
			if (degrees > 360 || degrees < -360) { degrees %= 360; }

			return cosList[degrees];
		}
		//-----------------------------------------------------------------------//

		// fun corner
		//-----------------------------------------------------------------------//

		//----- Num digits meta programming
		//-----| Gets the number of digits for a given number
		//-----| E.g. NumDigits<5467>::val = 4
		//-----| Note: This works for negative numbers as well.
		//-----|       > NumDigits<-1234>::val = 4
		template <long long N>
		struct NumDigits
		{
			static const long long val = 1 + NumDigits<GetPositive<N>() / 10>::val;
		};
		template <>
		struct NumDigits<0>
		{
			static const long long val = 0;
		};

		//----- Num digits function
		//-----| Function variant of the above
		static inline int NumDigitsFn(int Num)
		{
			int Total = 0;
			for (; Num > 0; Num /= 10, ++Total);
			return Total;
		}

		//----- Power function
		//-----| Function variant of the above
		template <typename T>
		static T PowFn(T Num, unsigned int N)
		{
			//----- 0 check
			if (!N) return 1;

			//----- Pow calculation
			T Result = Num;
			while (--N)
				Result *= Num;

			//----- Return
			return Result;
		}

		//----- Get digit at position function
		//-----| Eg. GetDigitAtPos(1234, 0) returns 1
		//-----|      GetDigitAtPos(1234, 1) returns 2
		//-----|      GetDigitAtPos(1234, 2) returns 3
		//-----|      GetDigitAtPos(1234, 3) returns 4
		static int GetDigitAtPos(int Digit, int Pos)
		{
			//----- Get number of digits
			//-----| Taking 1234 as eg, NumDigits is 4
			int totalDigits = NumDigitsFn(Digit);

			//----- Last pos check
			if (Pos == totalDigits - 1)
				return Digit % 10;

			//----- Get power mod
			//-----| using GetDigitAtPos(1234, 1) = 2 as eg,
			//-----| PowMod = 10^2 = 100
			int PowMod = PowFn(10, totalDigits - Pos - 1);

			//----- Deduct away unnecessary numbers
			//-----| This will leave us with 
			//-----|   1234 - (1234 % 100)
			//-----|	= 1234 - 34
			//-----| = 1200
			Digit -= Digit % PowMod;

			//----- Divide by PowMod
			//-----| This will leave us with 
			//-----|   1200 / 100
			//-----| = 12
			Digit /= PowMod;

			//----- Return desired value by modding with 10
			//-----| 12 % 10 = 2
			return Digit % 10;
		}
	};
}

#endif