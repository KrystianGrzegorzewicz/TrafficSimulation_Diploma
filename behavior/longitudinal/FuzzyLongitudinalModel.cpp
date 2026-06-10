#include "behavior/longitudinal/FuzzyLongitudinalModel.h"

#include <cmath>

// Number of inputs to the fuzzy inference system
const int fis_gcI = 4;
// Number of outputs to the fuzzy inference system
const int fis_gcO = 1;
// Number of rules to the fuzzy inference system
const int fis_gcR = 81;

FIS_TYPE g_fisInput[fis_gcI];
FIS_TYPE g_fisOutput[fis_gcO];

//***********************************************************************
// Support functions for Fuzzy Inference System
//***********************************************************************
// Gaussian Member Function
FIS_TYPE fis_gaussmf(FIS_TYPE x, FIS_TYPE* p)
{
	FIS_TYPE s = p[0], c = p[1];
	FIS_TYPE t = (x - c) / s;
	return exp(-(t * t) / 2);
}

FIS_TYPE fis_prod(FIS_TYPE a, FIS_TYPE b)
{
	return (a * b);
}

FIS_TYPE fis_probor(FIS_TYPE a, FIS_TYPE b)
{
	return (a + b - (a * b));
}

FIS_TYPE fis_sum(FIS_TYPE a, FIS_TYPE b)
{
	return (a + b);
}

FIS_TYPE fis_array_operation(FIS_TYPE* array, int size, _FIS_ARR_OP pfnOp)
{
	int i;
	FIS_TYPE ret = 0;

	if (size == 0) return ret;
	if (size == 1) return array[0];

	ret = array[0];
	for (i = 1; i < size; i++)
	{
		ret = (*pfnOp)(ret, array[i]);
	}

	return ret;
}

//***********************************************************************
// Data for Fuzzy Inference System
//***********************************************************************
// Pointers to the implementations of member functions
_FIS_MF fis_gMF[] =
{
	fis_gaussmf
};

// Count of member function for each Input
int fis_gIMFCount[] = { 3, 3, 3, 3 };

// Count of member function for each Output
int fis_gOMFCount[] = { 81 };

// Coefficients for the Input Member Functions
FIS_TYPE fis_gMFI0Coeff1[] = { 3.22658179980442, -0.218462333600012 };
FIS_TYPE fis_gMFI0Coeff2[] = { 3.60452322053568, 9.11341699360286 };
FIS_TYPE fis_gMFI0Coeff3[] = { 4.04528437069405, 17.9001274586771 };
FIS_TYPE* fis_gMFI0Coeff[] = { fis_gMFI0Coeff1, fis_gMFI0Coeff2, fis_gMFI0Coeff3 };
FIS_TYPE fis_gMFI1Coeff1[] = { 3.41619837120767, 1.09636931575301 };
FIS_TYPE fis_gMFI1Coeff2[] = { 3.17982849757995, 9.79832700285334 };
FIS_TYPE fis_gMFI1Coeff3[] = { 3.62730923144462, 17.8517632120278 };
FIS_TYPE* fis_gMFI1Coeff[] = { fis_gMFI1Coeff1, fis_gMFI1Coeff2, fis_gMFI1Coeff3 };
FIS_TYPE fis_gMFI2Coeff1[] = { 20.7125586563509, 2.15993114631788 };
FIS_TYPE fis_gMFI2Coeff2[] = { 20.7916094698847, 51.0588476902878 };
FIS_TYPE fis_gMFI2Coeff3[] = { 20.8656785431692, 99.9527824150979 };
FIS_TYPE* fis_gMFI2Coeff[] = { fis_gMFI2Coeff1, fis_gMFI2Coeff2, fis_gMFI2Coeff3 };
FIS_TYPE fis_gMFI3Coeff1[] = { 6.38063618132093, -13.5182767407886 };
FIS_TYPE fis_gMFI3Coeff2[] = { 6.38761478684417, 1.66687882685567 };
FIS_TYPE fis_gMFI3Coeff3[] = { 6.6518207891622, 16.6423850081796 };
FIS_TYPE* fis_gMFI3Coeff[] = { fis_gMFI3Coeff1, fis_gMFI3Coeff2, fis_gMFI3Coeff3 };
FIS_TYPE** fis_gMFICoeff[] = { fis_gMFI0Coeff, fis_gMFI1Coeff, fis_gMFI2Coeff, fis_gMFI3Coeff };

// Coefficients for the Output Member Functions
FIS_TYPE fis_gMFO0Coeff1[] = { 27.9109391893486, -201.785371900776, -47.7819466418435, 65.8228770140397, 1486.66669147197 };
FIS_TYPE fis_gMFO0Coeff2[] = { -22.8434328137327, 21.9059549149887, 21.5536973040095, 39.5867269602636, -47.2214311665054 };
FIS_TYPE fis_gMFO0Coeff3[] = { 220.963761022924, -162.407321642475, -228.195765388139, 156.453243882237, -1653.82331671312 };
FIS_TYPE fis_gMFO0Coeff4[] = { -36.5723182734038, 265.099789186651, -26.8293465619779, 153.139994432476, 3357.29053776985 };
FIS_TYPE fis_gMFO0Coeff5[] = { 47.3248953275695, -13.6280349816172, 34.4943797914265, 196.61901411902, -1108.67719992543 };
FIS_TYPE fis_gMFO0Coeff6[] = { -224.217652415484, 517.328538164472, -433.73643566663, 2209.70062952535, 947.127010842026 };
FIS_TYPE fis_gMFO0Coeff7[] = { -596.657088962394, -536.850206312488, -393.260334712883, -222.718246483146, -357.176137137645 };
FIS_TYPE fis_gMFO0Coeff8[] = { 70.1976905975768, 54.6165794504694, 75.0304333346341, 386.59682549007, -2401.19363277388 };
FIS_TYPE fis_gMFO0Coeff9[] = { -8.72728864109208, 67.0954855284816, -171.635235285569, -79.0518515219815, -114.929922668911 };
FIS_TYPE fis_gMFO0Coeff10[] = { 17.7363358393029, -60.3503442424279, 9.02846932913851, -16.5335895352843, 456.628846624387 };
FIS_TYPE fis_gMFO0Coeff11[] = { 1.46317245731367, 0.218525736965659, -2.39850322067966, 4.85508475629218, 40.4466715538333 };
FIS_TYPE fis_gMFO0Coeff12[] = { -49.2100579367103, -24.4340300330147, 18.5904065122748, 185.984830037548, -771.987842676747 };
FIS_TYPE fis_gMFO0Coeff13[] = { -21.2109885102485, 91.8257184936213, 21.1300123302113, -7.80867864128304, -1898.62658195934 };
FIS_TYPE fis_gMFO0Coeff14[] = { -21.983202411468, -0.00955021835364162, -3.34473600673887, -44.955473942473, 26.4795295790388 };
FIS_TYPE fis_gMFO0Coeff15[] = { 340.937520730186, 353.011066953149, 0.394798641086539, -870.355762965924, 1093.08694466128 };
FIS_TYPE fis_gMFO0Coeff16[] = { -255.230558579236, -2014.01878367924, 323.824104884333, -207.993428549768, -129.280441472613 };
FIS_TYPE fis_gMFO0Coeff17[] = { 31.9976225488163, 230.887870496054, 2.36758488068566, 413.144936931868, -1334.13215738227 };
FIS_TYPE fis_gMFO0Coeff18[] = { -80.3095704786587, -262.62228926678, -534.77057560446, -150.30725940449, -58.6447050607633 };
FIS_TYPE fis_gMFO0Coeff19[] = { 169.098005429912, -147.646686128181, 2.50575349605588, 77.3775867962923, 1884.24470957862 };
FIS_TYPE fis_gMFO0Coeff20[] = { -52.8830136941281, 4.56489460027685, 16.7113902893726, 10.1054373469037, 190.718419510206 };
FIS_TYPE fis_gMFO0Coeff21[] = { 312.128118434481, -122.244885663901, -101.005599451543, -105.502358840474, -513.077769380816 };
FIS_TYPE fis_gMFO0Coeff22[] = { -291.688235777203, 36.5391396986255, -0.805852155878802, 36.5562702394565, 1220.15213409308 };
FIS_TYPE fis_gMFO0Coeff23[] = { 135.048598985382, -94.8005599199558, 44.34847955991, -58.546120132124, -1611.0265505804 };
FIS_TYPE fis_gMFO0Coeff24[] = { -457.719311541967, 1731.9628622082, -561.73565329073, -240.140510946908, -255.859347808237 };
FIS_TYPE fis_gMFO0Coeff25[] = { -1144.50377788905, -50.1137007847663, 154.595966663601, 90.0327939987552, -80.4248757328342 };
FIS_TYPE fis_gMFO0Coeff26[] = { 116.94459636846, -44.6364689823765, -72.9337230677918, 415.208217156882, -264.106905550816 };
FIS_TYPE fis_gMFO0Coeff27[] = { 354.337788195062, 593.192343560143, 1346.10712132406, 403.17594432829, 16.705238247089 };
FIS_TYPE fis_gMFO0Coeff28[] = { -102.151148412048, 61.4483843391879, 24.8658465774804, 36.2965201902277, 193.473941362954 };
FIS_TYPE fis_gMFO0Coeff29[] = { 1.5593217810954, -8.30823356578428, -3.48981119524905, 10.9860748436741, 82.4621787911479 };
FIS_TYPE fis_gMFO0Coeff30[] = { 95.4116338123747, 79.6477812526435, 20.8710452509841, 43.0045607292293, -1582.07163219534 };
FIS_TYPE fis_gMFO0Coeff31[] = { 253.31991554211, -72.5362399902611, -2.27647986218966, 2.51027029423012, -1137.4975341 };
FIS_TYPE fis_gMFO0Coeff32[] = { -5.3912288275991, 2.25777310797644, -0.11982624735533, 40.5418458785768, 140.083751698529 };
FIS_TYPE fis_gMFO0Coeff33[] = { 156.893090519284, -70.2398548487896, 10.6840047773917, 153.974343388901, -3212.08590090887 };
FIS_TYPE fis_gMFO0Coeff34[] = { -486.918331014922, -1144.14780841202, 109.422439016701, -1065.00407916557, -19.4899401617775 };
FIS_TYPE fis_gMFO0Coeff35[] = { 71.6853290162164, 120.640953203697, -20.9159994308036, -294.297703250711, -528.895041300431 };
FIS_TYPE fis_gMFO0Coeff36[] = { -115.924470614974, 124.678763666879, 296.899736153026, 359.099986348047, 7.92639161924349 };
FIS_TYPE fis_gMFO0Coeff37[] = { 14.9336252847615, -7.65348188412823, -3.80828247716013, -7.98068866337944, -95.693274422765 };
FIS_TYPE fis_gMFO0Coeff38[] = { -4.95228675606839, 2.17471117421511, 1.38935629438718, -2.49327766707576, 8.69271254120435 };
FIS_TYPE fis_gMFO0Coeff39[] = { 17.1971449642439, 1.03844889527174, -2.21940087410171, -8.12261502237795, -84.3903815391545 };
FIS_TYPE fis_gMFO0Coeff40[] = { -21.5236888640711, -11.3237934122351, -2.44415954592022, 2.75068038500222, 411.742119514619 };
FIS_TYPE fis_gMFO0Coeff41[] = { 4.3242075629052, -3.4534480155748, 1.25225187465017, -7.10940277780398, -68.0938653739239 };
FIS_TYPE fis_gMFO0Coeff42[] = { -19.0086178894033, -2.91277898953381, -1.25850501466333, -11.2001480454845, 508.666223941821 };
FIS_TYPE fis_gMFO0Coeff43[] = { 70.0962986084848, 129.459002938564, -13.1690771741738, 108.912986487085, 446.407194582869 };
FIS_TYPE fis_gMFO0Coeff44[] = { -28.070134468792, -4.58273250645302, 5.55693780251414, 79.2498683049416, 9.06562887914866 };
FIS_TYPE fis_gMFO0Coeff45[] = { 475.657486509321, -252.042767510869, -99.4169064194758, 230.971501512673, -220.025092755351 };
FIS_TYPE fis_gMFO0Coeff46[] = { -17.68346999341, 19.1908389666991, -1.62940272486336, -15.357588572101, -302.992482482163 };
FIS_TYPE fis_gMFO0Coeff47[] = { -1.02128234864331, 2.44408982304084, 0.314348757787033, -6.07020392205604, -22.138414934661 };
FIS_TYPE fis_gMFO0Coeff48[] = { 15.9271724014295, -9.37295763714412, 0.802771114174701, -7.65444202830128, 85.2138842508702 };
FIS_TYPE fis_gMFO0Coeff49[] = { -4.70777206300379, -8.92545423430226, -2.21256254650592, -8.71252966562724, 227.898813677468 };
FIS_TYPE fis_gMFO0Coeff50[] = { -0.994955632703361, -3.33851297899436, 0.170149178578501, -3.44260060194349, 69.3607446092597 };
FIS_TYPE fis_gMFO0Coeff51[] = { 4.72332434280717, 6.70061487725099, -0.0762854262002143, -5.54308567569392, -82.9744017710311 };
FIS_TYPE fis_gMFO0Coeff52[] = { -10.3658676198655, 54.6951686285512, -3.46767180378709, 44.707477689991, 114.992501795107 };
FIS_TYPE fis_gMFO0Coeff53[] = { -1.52117545888221, -5.67080544278432, 0.0815859778015579, 4.96187232393657, 43.4172330758648 };
FIS_TYPE fis_gMFO0Coeff54[] = { 29.0451799657581, -6.5202134008772, 3.65918837622585, 10.7078197692547, -559.510795570023 };
FIS_TYPE fis_gMFO0Coeff55[] = { 1897.81330776742, -1336.19491281142, -405.886869807731, -540.223752206331, 96.0659184001903 };
FIS_TYPE fis_gMFO0Coeff56[] = { -78.8332570322983, 290.118401250767, -7.34530020093325, -123.441707519773, -1223.71407343365 };
FIS_TYPE fis_gMFO0Coeff57[] = { 513.447099927675, -1227.65687548971, -105.902692085569, 244.166356559501, 145.460874902861 };
FIS_TYPE fis_gMFO0Coeff58[] = { -205.517574542294, -377.636531541384, 52.7097211645596, 763.721511518948, -137.582629175373 };
FIS_TYPE fis_gMFO0Coeff59[] = { 167.685294321488, 192.471401921075, -26.0081426851484, 7.19533269596483, -2269.74078501056 };
FIS_TYPE fis_gMFO0Coeff60[] = { 1707.54240788008, -1036.06508659358, -297.256094349797, -340.32969587109, 159.686127179809 };
FIS_TYPE fis_gMFO0Coeff61[] = { -33.7448374263948, -133.378905317909, -533.030258051503, 103.508776736109, -27.6850131876132 };
FIS_TYPE fis_gMFO0Coeff62[] = { 219.561773946379, -95.44979480742, 18.0383226382919, -328.097316831746, -114.793858775964 };
FIS_TYPE fis_gMFO0Coeff63[] = { 213.090477961133, 51.0489531728338, 460.188087850791, 152.968173781176, -6.98755147574664 };
FIS_TYPE fis_gMFO0Coeff64[] = { -432.596624220082, 327.683086551354, 36.5601608925447, 16.2026440088163, 78.3197929594925 };
FIS_TYPE fis_gMFO0Coeff65[] = { -17.0987432964476, -8.37012648220536, 3.27601089819899, -22.5190584218976, 346.726271611681 };
FIS_TYPE fis_gMFO0Coeff66[] = { 44.246298639971, -35.4421124653332, -13.7776053304984, -20.6547698393544, 334.792750073097 };
FIS_TYPE fis_gMFO0Coeff67[] = { 115.669512454576, -120.191026134156, 16.2892946336441, -70.6321280053247, -673.39298255546 };
FIS_TYPE fis_gMFO0Coeff68[] = { 3.71450654760132, 20.7934014873541, 2.30092298096319, 17.4874575145915, -430.391472807507 };
FIS_TYPE fis_gMFO0Coeff69[] = { -26.419793348884, -3.23078738949239, -0.486970075927349, 25.2364671789241, 143.08763443695 };
FIS_TYPE fis_gMFO0Coeff70[] = { 586.527920401984, -636.873362951834, -141.370063805078, -468.85733475075, -298.683270678563 };
FIS_TYPE fis_gMFO0Coeff71[] = { -79.3078845853579, 78.8682608593893, 27.4154508916802, -173.423429285463, -1105.66277277369 };
FIS_TYPE fis_gMFO0Coeff72[] = { 284.483263503092, -83.0978558409944, -29.2275959399242, -185.301528208841, 554.450128296687 };
FIS_TYPE fis_gMFO0Coeff73[] = { 18.7620445210724, -31.7162544636165, 5.86102350297586, 68.8466758767466, 896.934386913971 };
FIS_TYPE fis_gMFO0Coeff74[] = { -1.68433475451124, 0.272523947639438, -1.07851560289986, 17.4561428961913, -22.8923482904157 };
FIS_TYPE fis_gMFO0Coeff75[] = { 22.2156670612888, -13.922423042212, 3.37496895652615, 19.082638332614, -429.624798960462 };
FIS_TYPE fis_gMFO0Coeff76[] = { -7.16665021865857, 15.6165558707916, 3.72524758548843, 8.13613754983137, -307.300020335661 };
FIS_TYPE fis_gMFO0Coeff77[] = { -1.43471128267156, 4.00225055297636, -0.675503020155899, -0.762179912731764, -20.4425328886012 };
FIS_TYPE fis_gMFO0Coeff78[] = { 1.73255388383956, 0.0288891905319953, 1.86133897864192, -1.35063722766601, -108.649970501031 };
FIS_TYPE fis_gMFO0Coeff79[] = { 26.3900284096182, -90.5328905888141, 19.9413130427057, -7.13219049346195, -7.39871490824737 };
FIS_TYPE fis_gMFO0Coeff80[] = { -3.9219699310772, 12.9646328391005, -2.86310606272725, 7.95224950506753, 32.967200336251 };
FIS_TYPE fis_gMFO0Coeff81[] = { -0.882922122812033, -38.4424607844415, 5.58965562275267, 7.52174365830103, 143.261244580388 };
FIS_TYPE* fis_gMFO0Coeff[] = { fis_gMFO0Coeff1, fis_gMFO0Coeff2, fis_gMFO0Coeff3, fis_gMFO0Coeff4, fis_gMFO0Coeff5, fis_gMFO0Coeff6, fis_gMFO0Coeff7, fis_gMFO0Coeff8, fis_gMFO0Coeff9, fis_gMFO0Coeff10, fis_gMFO0Coeff11, fis_gMFO0Coeff12, fis_gMFO0Coeff13, fis_gMFO0Coeff14, fis_gMFO0Coeff15, fis_gMFO0Coeff16, fis_gMFO0Coeff17, fis_gMFO0Coeff18, fis_gMFO0Coeff19, fis_gMFO0Coeff20, fis_gMFO0Coeff21, fis_gMFO0Coeff22, fis_gMFO0Coeff23, fis_gMFO0Coeff24, fis_gMFO0Coeff25, fis_gMFO0Coeff26, fis_gMFO0Coeff27, fis_gMFO0Coeff28, fis_gMFO0Coeff29, fis_gMFO0Coeff30, fis_gMFO0Coeff31, fis_gMFO0Coeff32, fis_gMFO0Coeff33, fis_gMFO0Coeff34, fis_gMFO0Coeff35, fis_gMFO0Coeff36, fis_gMFO0Coeff37, fis_gMFO0Coeff38, fis_gMFO0Coeff39, fis_gMFO0Coeff40, fis_gMFO0Coeff41, fis_gMFO0Coeff42, fis_gMFO0Coeff43, fis_gMFO0Coeff44, fis_gMFO0Coeff45, fis_gMFO0Coeff46, fis_gMFO0Coeff47, fis_gMFO0Coeff48, fis_gMFO0Coeff49, fis_gMFO0Coeff50, fis_gMFO0Coeff51, fis_gMFO0Coeff52, fis_gMFO0Coeff53, fis_gMFO0Coeff54, fis_gMFO0Coeff55, fis_gMFO0Coeff56, fis_gMFO0Coeff57, fis_gMFO0Coeff58, fis_gMFO0Coeff59, fis_gMFO0Coeff60, fis_gMFO0Coeff61, fis_gMFO0Coeff62, fis_gMFO0Coeff63, fis_gMFO0Coeff64, fis_gMFO0Coeff65, fis_gMFO0Coeff66, fis_gMFO0Coeff67, fis_gMFO0Coeff68, fis_gMFO0Coeff69, fis_gMFO0Coeff70, fis_gMFO0Coeff71, fis_gMFO0Coeff72, fis_gMFO0Coeff73, fis_gMFO0Coeff74, fis_gMFO0Coeff75, fis_gMFO0Coeff76, fis_gMFO0Coeff77, fis_gMFO0Coeff78, fis_gMFO0Coeff79, fis_gMFO0Coeff80, fis_gMFO0Coeff81 };
FIS_TYPE** fis_gMFOCoeff[] = { fis_gMFO0Coeff };

// Input membership function set
int fis_gMFI0[] = { 0, 0, 0 };
int fis_gMFI1[] = { 0, 0, 0 };
int fis_gMFI2[] = { 0, 0, 0 };
int fis_gMFI3[] = { 0, 0, 0 };
int* fis_gMFI[] = { fis_gMFI0, fis_gMFI1, fis_gMFI2, fis_gMFI3 };

// Output membership function set

//int* fis_gMFO[5] = {};

// Rule Weights
FIS_TYPE fis_gRWeight[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };

// Rule Type
int fis_gRType[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };

// Rule Inputs
int fis_gRI0[] = { 1, 1, 1, 1 };
int fis_gRI1[] = { 1, 1, 1, 2 };
int fis_gRI2[] = { 1, 1, 1, 3 };
int fis_gRI3[] = { 1, 1, 2, 1 };
int fis_gRI4[] = { 1, 1, 2, 2 };
int fis_gRI5[] = { 1, 1, 2, 3 };
int fis_gRI6[] = { 1, 1, 3, 1 };
int fis_gRI7[] = { 1, 1, 3, 2 };
int fis_gRI8[] = { 1, 1, 3, 3 };
int fis_gRI9[] = { 1, 2, 1, 1 };
int fis_gRI10[] = { 1, 2, 1, 2 };
int fis_gRI11[] = { 1, 2, 1, 3 };
int fis_gRI12[] = { 1, 2, 2, 1 };
int fis_gRI13[] = { 1, 2, 2, 2 };
int fis_gRI14[] = { 1, 2, 2, 3 };
int fis_gRI15[] = { 1, 2, 3, 1 };
int fis_gRI16[] = { 1, 2, 3, 2 };
int fis_gRI17[] = { 1, 2, 3, 3 };
int fis_gRI18[] = { 1, 3, 1, 1 };
int fis_gRI19[] = { 1, 3, 1, 2 };
int fis_gRI20[] = { 1, 3, 1, 3 };
int fis_gRI21[] = { 1, 3, 2, 1 };
int fis_gRI22[] = { 1, 3, 2, 2 };
int fis_gRI23[] = { 1, 3, 2, 3 };
int fis_gRI24[] = { 1, 3, 3, 1 };
int fis_gRI25[] = { 1, 3, 3, 2 };
int fis_gRI26[] = { 1, 3, 3, 3 };
int fis_gRI27[] = { 2, 1, 1, 1 };
int fis_gRI28[] = { 2, 1, 1, 2 };
int fis_gRI29[] = { 2, 1, 1, 3 };
int fis_gRI30[] = { 2, 1, 2, 1 };
int fis_gRI31[] = { 2, 1, 2, 2 };
int fis_gRI32[] = { 2, 1, 2, 3 };
int fis_gRI33[] = { 2, 1, 3, 1 };
int fis_gRI34[] = { 2, 1, 3, 2 };
int fis_gRI35[] = { 2, 1, 3, 3 };
int fis_gRI36[] = { 2, 2, 1, 1 };
int fis_gRI37[] = { 2, 2, 1, 2 };
int fis_gRI38[] = { 2, 2, 1, 3 };
int fis_gRI39[] = { 2, 2, 2, 1 };
int fis_gRI40[] = { 2, 2, 2, 2 };
int fis_gRI41[] = { 2, 2, 2, 3 };
int fis_gRI42[] = { 2, 2, 3, 1 };
int fis_gRI43[] = { 2, 2, 3, 2 };
int fis_gRI44[] = { 2, 2, 3, 3 };
int fis_gRI45[] = { 2, 3, 1, 1 };
int fis_gRI46[] = { 2, 3, 1, 2 };
int fis_gRI47[] = { 2, 3, 1, 3 };
int fis_gRI48[] = { 2, 3, 2, 1 };
int fis_gRI49[] = { 2, 3, 2, 2 };
int fis_gRI50[] = { 2, 3, 2, 3 };
int fis_gRI51[] = { 2, 3, 3, 1 };
int fis_gRI52[] = { 2, 3, 3, 2 };
int fis_gRI53[] = { 2, 3, 3, 3 };
int fis_gRI54[] = { 3, 1, 1, 1 };
int fis_gRI55[] = { 3, 1, 1, 2 };
int fis_gRI56[] = { 3, 1, 1, 3 };
int fis_gRI57[] = { 3, 1, 2, 1 };
int fis_gRI58[] = { 3, 1, 2, 2 };
int fis_gRI59[] = { 3, 1, 2, 3 };
int fis_gRI60[] = { 3, 1, 3, 1 };
int fis_gRI61[] = { 3, 1, 3, 2 };
int fis_gRI62[] = { 3, 1, 3, 3 };
int fis_gRI63[] = { 3, 2, 1, 1 };
int fis_gRI64[] = { 3, 2, 1, 2 };
int fis_gRI65[] = { 3, 2, 1, 3 };
int fis_gRI66[] = { 3, 2, 2, 1 };
int fis_gRI67[] = { 3, 2, 2, 2 };
int fis_gRI68[] = { 3, 2, 2, 3 };
int fis_gRI69[] = { 3, 2, 3, 1 };
int fis_gRI70[] = { 3, 2, 3, 2 };
int fis_gRI71[] = { 3, 2, 3, 3 };
int fis_gRI72[] = { 3, 3, 1, 1 };
int fis_gRI73[] = { 3, 3, 1, 2 };
int fis_gRI74[] = { 3, 3, 1, 3 };
int fis_gRI75[] = { 3, 3, 2, 1 };
int fis_gRI76[] = { 3, 3, 2, 2 };
int fis_gRI77[] = { 3, 3, 2, 3 };
int fis_gRI78[] = { 3, 3, 3, 1 };
int fis_gRI79[] = { 3, 3, 3, 2 };
int fis_gRI80[] = { 3, 3, 3, 3 };
int* fis_gRI[] = { fis_gRI0, fis_gRI1, fis_gRI2, fis_gRI3, fis_gRI4, fis_gRI5, fis_gRI6, fis_gRI7, fis_gRI8, fis_gRI9, fis_gRI10, fis_gRI11, fis_gRI12, fis_gRI13, fis_gRI14, fis_gRI15, fis_gRI16, fis_gRI17, fis_gRI18, fis_gRI19, fis_gRI20, fis_gRI21, fis_gRI22, fis_gRI23, fis_gRI24, fis_gRI25, fis_gRI26, fis_gRI27, fis_gRI28, fis_gRI29, fis_gRI30, fis_gRI31, fis_gRI32, fis_gRI33, fis_gRI34, fis_gRI35, fis_gRI36, fis_gRI37, fis_gRI38, fis_gRI39, fis_gRI40, fis_gRI41, fis_gRI42, fis_gRI43, fis_gRI44, fis_gRI45, fis_gRI46, fis_gRI47, fis_gRI48, fis_gRI49, fis_gRI50, fis_gRI51, fis_gRI52, fis_gRI53, fis_gRI54, fis_gRI55, fis_gRI56, fis_gRI57, fis_gRI58, fis_gRI59, fis_gRI60, fis_gRI61, fis_gRI62, fis_gRI63, fis_gRI64, fis_gRI65, fis_gRI66, fis_gRI67, fis_gRI68, fis_gRI69, fis_gRI70, fis_gRI71, fis_gRI72, fis_gRI73, fis_gRI74, fis_gRI75, fis_gRI76, fis_gRI77, fis_gRI78, fis_gRI79, fis_gRI80 };

// Rule Outputs
int fis_gRO0[] = { 1 };
int fis_gRO1[] = { 2 };
int fis_gRO2[] = { 3 };
int fis_gRO3[] = { 4 };
int fis_gRO4[] = { 5 };
int fis_gRO5[] = { 6 };
int fis_gRO6[] = { 7 };
int fis_gRO7[] = { 8 };
int fis_gRO8[] = { 9 };
int fis_gRO9[] = { 10 };
int fis_gRO10[] = { 11 };
int fis_gRO11[] = { 12 };
int fis_gRO12[] = { 13 };
int fis_gRO13[] = { 14 };
int fis_gRO14[] = { 15 };
int fis_gRO15[] = { 16 };
int fis_gRO16[] = { 17 };
int fis_gRO17[] = { 18 };
int fis_gRO18[] = { 19 };
int fis_gRO19[] = { 20 };
int fis_gRO20[] = { 21 };
int fis_gRO21[] = { 22 };
int fis_gRO22[] = { 23 };
int fis_gRO23[] = { 24 };
int fis_gRO24[] = { 25 };
int fis_gRO25[] = { 26 };
int fis_gRO26[] = { 27 };
int fis_gRO27[] = { 28 };
int fis_gRO28[] = { 29 };
int fis_gRO29[] = { 30 };
int fis_gRO30[] = { 31 };
int fis_gRO31[] = { 32 };
int fis_gRO32[] = { 33 };
int fis_gRO33[] = { 34 };
int fis_gRO34[] = { 35 };
int fis_gRO35[] = { 36 };
int fis_gRO36[] = { 37 };
int fis_gRO37[] = { 38 };
int fis_gRO38[] = { 39 };
int fis_gRO39[] = { 40 };
int fis_gRO40[] = { 41 };
int fis_gRO41[] = { 42 };
int fis_gRO42[] = { 43 };
int fis_gRO43[] = { 44 };
int fis_gRO44[] = { 45 };
int fis_gRO45[] = { 46 };
int fis_gRO46[] = { 47 };
int fis_gRO47[] = { 48 };
int fis_gRO48[] = { 49 };
int fis_gRO49[] = { 50 };
int fis_gRO50[] = { 51 };
int fis_gRO51[] = { 52 };
int fis_gRO52[] = { 53 };
int fis_gRO53[] = { 54 };
int fis_gRO54[] = { 55 };
int fis_gRO55[] = { 56 };
int fis_gRO56[] = { 57 };
int fis_gRO57[] = { 58 };
int fis_gRO58[] = { 59 };
int fis_gRO59[] = { 60 };
int fis_gRO60[] = { 61 };
int fis_gRO61[] = { 62 };
int fis_gRO62[] = { 63 };
int fis_gRO63[] = { 64 };
int fis_gRO64[] = { 65 };
int fis_gRO65[] = { 66 };
int fis_gRO66[] = { 67 };
int fis_gRO67[] = { 68 };
int fis_gRO68[] = { 69 };
int fis_gRO69[] = { 70 };
int fis_gRO70[] = { 71 };
int fis_gRO71[] = { 72 };
int fis_gRO72[] = { 73 };
int fis_gRO73[] = { 74 };
int fis_gRO74[] = { 75 };
int fis_gRO75[] = { 76 };
int fis_gRO76[] = { 77 };
int fis_gRO77[] = { 78 };
int fis_gRO78[] = { 79 };
int fis_gRO79[] = { 80 };
int fis_gRO80[] = { 81 };
int* fis_gRO[] = { fis_gRO0, fis_gRO1, fis_gRO2, fis_gRO3, fis_gRO4, fis_gRO5, fis_gRO6, fis_gRO7, fis_gRO8, fis_gRO9, fis_gRO10, fis_gRO11, fis_gRO12, fis_gRO13, fis_gRO14, fis_gRO15, fis_gRO16, fis_gRO17, fis_gRO18, fis_gRO19, fis_gRO20, fis_gRO21, fis_gRO22, fis_gRO23, fis_gRO24, fis_gRO25, fis_gRO26, fis_gRO27, fis_gRO28, fis_gRO29, fis_gRO30, fis_gRO31, fis_gRO32, fis_gRO33, fis_gRO34, fis_gRO35, fis_gRO36, fis_gRO37, fis_gRO38, fis_gRO39, fis_gRO40, fis_gRO41, fis_gRO42, fis_gRO43, fis_gRO44, fis_gRO45, fis_gRO46, fis_gRO47, fis_gRO48, fis_gRO49, fis_gRO50, fis_gRO51, fis_gRO52, fis_gRO53, fis_gRO54, fis_gRO55, fis_gRO56, fis_gRO57, fis_gRO58, fis_gRO59, fis_gRO60, fis_gRO61, fis_gRO62, fis_gRO63, fis_gRO64, fis_gRO65, fis_gRO66, fis_gRO67, fis_gRO68, fis_gRO69, fis_gRO70, fis_gRO71, fis_gRO72, fis_gRO73, fis_gRO74, fis_gRO75, fis_gRO76, fis_gRO77, fis_gRO78, fis_gRO79, fis_gRO80 };

// Input range Min
FIS_TYPE fis_gIMin[] = { 0.0287288, 1.02055, 2.23483, -13.5636 };

// Input range Max
FIS_TYPE fis_gIMax[] = { 17.9921, 18, 100, 16.8005 };

// Output range Min
FIS_TYPE fis_gOMin[] = { -5.5 };

// Output range Max
FIS_TYPE fis_gOMax[] = { 3.5 };

//***********************************************************************
// Data dependent support functions for Fuzzy Inference System
//***********************************************************************
// None for Sugeno

//***********************************************************************
// Fuzzy Inference System
//***********************************************************************
void fis_evaluate()
{
	FIS_TYPE fuzzyInput0[] = { 0, 0, 0 };
	FIS_TYPE fuzzyInput1[] = { 0, 0, 0 };
	FIS_TYPE fuzzyInput2[] = { 0, 0, 0 };
	FIS_TYPE fuzzyInput3[] = { 0, 0, 0 };
	FIS_TYPE* fuzzyInput[fis_gcI] = { fuzzyInput0, fuzzyInput1, fuzzyInput2, fuzzyInput3, };
	FIS_TYPE fuzzyOutput0[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	FIS_TYPE* fuzzyOutput[fis_gcO] = { fuzzyOutput0, };
	FIS_TYPE fuzzyRules[fis_gcR] = { 0 };
	FIS_TYPE fuzzyFires[fis_gcR] = { 0 };
	FIS_TYPE* fuzzyRuleSet[] = { fuzzyRules, fuzzyFires };
	FIS_TYPE sW = 0;

	// Transforming input to fuzzy Input
	int i, j, r, o;
	for (i = 0; i < fis_gcI; ++i)
	{
		for (j = 0; j < fis_gIMFCount[i]; ++j)
		{
			fuzzyInput[i][j] =
				(fis_gMF[fis_gMFI[i][j]])(g_fisInput[i], fis_gMFICoeff[i][j]);
		}
	}

	int index = 0;
	for (r = 0; r < fis_gcR; ++r)
	{
		if (fis_gRType[r] == 1)
		{
			fuzzyFires[r] = 1;
			for (i = 0; i < fis_gcI; ++i)
			{
				index = fis_gRI[r][i];
				if (index > 0)
					fuzzyFires[r] = fis_prod(fuzzyFires[r], fuzzyInput[i][index - 1]);
				else if (index < 0)
					fuzzyFires[r] = fis_prod(fuzzyFires[r], 1 - fuzzyInput[i][-index - 1]);
				else
					fuzzyFires[r] = fis_prod(fuzzyFires[r], 1);
			}
		}
		else
		{
			fuzzyFires[r] = 0;
			for (i = 0; i < fis_gcI; ++i)
			{
				index = fis_gRI[r][i];
				if (index > 0)
					fuzzyFires[r] = fis_probor(fuzzyFires[r], fuzzyInput[i][index - 1]);
				else if (index < 0)
					fuzzyFires[r] = fis_probor(fuzzyFires[r], 1 - fuzzyInput[i][-index - 1]);
				else
					fuzzyFires[r] = fis_probor(fuzzyFires[r], 0);
			}
		}

		fuzzyFires[r] = fis_gRWeight[r] * fuzzyFires[r];
		sW += fuzzyFires[r];
	}

	if (sW == 0)
	{
		for (o = 0; o < fis_gcO; ++o)
		{
			g_fisOutput[o] = ((fis_gOMax[o] + fis_gOMin[o]) / 2);
		}
	}
	else
	{
		for (o = 0; o < fis_gcO; ++o)
		{
			FIS_TYPE sWI = 0.0;
			for (j = 0; j < fis_gOMFCount[o]; ++j)
			{
				fuzzyOutput[o][j] = fis_gMFOCoeff[o][j][fis_gcI];
				for (i = 0; i < fis_gcI; ++i)
				{
					fuzzyOutput[o][j] += g_fisInput[i] * fis_gMFOCoeff[o][j][i];
				}
			}

			for (r = 0; r < fis_gcR; ++r)
			{
				index = fis_gRO[r][o] - 1;
				sWI += fuzzyFires[r] * fuzzyOutput[o][index];
			}

			g_fisOutput[o] = sWI / sW;
		}
	}
}

float FuzzyLongitudinalModel::computeAcceleration(
	const CarState& self,
	const PerceptionState& perception,
	float desiredSpeed,
	float maxAccel,
	float maxDecel
)
{
	g_fisInput[0] = self.velocity.length();
	g_fisInput[1] = desiredSpeed;
	perception.distanceToCarAhead > 100.0f ? g_fisInput[2] = 100.0f : g_fisInput[2] = perception.distanceToCarAhead;
	g_fisInput[3] = perception.relativeSpeed;

	g_fisOutput[0] = 0;

	fis_evaluate();

	return g_fisOutput[0];
}