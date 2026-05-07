#include "TJunction.h"
#include <random>
#include <iostream>

TJunction::TJunction() {
    std::vector<Vec2> travelPoints1;
    std::vector<Vec2> travelPoints2;
    std::vector<Vec2> travelPoints3;
    Travel tempTravel(travelPoints1, 1, 0);

    //z prawej do lewej
        travelPoints1 = {
            Vec2(100.0f, -1.75f),
            Vec2(100.0f, -1.75f),
            Vec2(90.0f, -1.75f),
            Vec2(60.0f, -1.75f),
            Vec2(30.0f, -1.75f),
            Vec2(5.0f, -1.75f),
             Vec2(-5.0f, -1.75f),

            Vec2(-30.0f, -1.75f),
            Vec2(-60.0f, -1.75f),
            Vec2(-90.0f, -1.75f),
            Vec2(-100.0f, -1.75f),
            Vec2(-100.0f, -1.75f)
        };
        travels.push_back(Travel(travelPoints1, 2, 1));
		//z lewej do prawej
        travelPoints2 = {
            Vec2(-100.0f, 1.75f),
            Vec2(-100.0f, 1.75f),
            Vec2(-90.0f, 1.75f),
            Vec2(-60.0f, 1.75f),

            Vec2(-40.0f, 1.75f),
            Vec2(-35.0f, 1.75f),
            Vec2(-30.0f, 3.5f),
            Vec2(-25.0f, 5.25f),
            Vec2(-20.0f, 5.25f),

            Vec2(-5.0f, 5.25f),
            Vec2(3.5f, 5.25f),
            Vec2(8.5f, 5.25f),
            Vec2(13.5f, 3.5f),
            Vec2(18.5f, 1.75f),
            Vec2(23.5f, 1.75f),

            Vec2(40.0f, 1.75f),
            Vec2(60.0f, 1.75f),
            Vec2(90.0f, 1.75f),
            Vec2(100.0f, 1.75f),
            Vec2(100.0f, 1.75f)
        };
        travels.push_back(Travel(travelPoints2, 2, 2));
		//z lewej w dol
        travelPoints3 = {
            Vec2(-100.0f, 1.75f),
            Vec2(-100.0f, 1.75f),
            Vec2(-90.0f, 1.75f),
            Vec2(-60.0f, 1.75f),
            Vec2(-30.0f, 1.75f),
            Vec2(-10.0f, 1.75f),

            Vec2(-2.5f, 1.75f),
            Vec2(0.0f, 1.75f),
            Vec2(1.75f, 1.75f),
            Vec2(1.75f, -3.5f),
            Vec2(1.75f, -5.0f),

            Vec2(1.75f, -5.0f),
            Vec2(1.75f, -20.0f),
            Vec2(1.75f, -50.0f),
            Vec2(1.75f, -70.0f),
            Vec2(1.75f, -100.0f),
            Vec2(1.75f, -100.0f),
        };
        travels.push_back(Travel(travelPoints3, 1, 3));

}
TJunction::TJunction(int index) {
    std::vector<Vec2> travelPoints1;
    std::vector<Vec2> travelPoints2;
    std::vector<Vec2> travelPoints3;
    std::vector<Vec2> travelPoints4;
    std::vector<Vec2> travelPoints5;
    std::vector<Vec2> travelPoints6;
    Travel tempTravel(travelPoints1, 1, 0);
    const float STEP = 0.05f; // gęstość punktów

    switch (index) {
        case 0:
            // Default T-junction
            break;
        case 1: //bezkonfliktowy prawoskret
            blocks.push_back(Block(-100.0f, -4.0f, 100.0f, -3.5f));
            blocks.push_back(Block(-100.0f, 3.5f, -3.5f, 4.0f));
            blocks.push_back(Block(3.5f, 3.5f, 100.0f, 4.0f));
            blocks.push_back(Block(-4.0f, 3.5f, -3.5f, 50.0f));
            blocks.push_back(Block(3.5f, 3.5f, 4.0f, 50.0f));
            
            // Add lines for junction markings
            lines.push_back(Line(-100.0f, -3.75f, 100.0f, -3.75f, 0.3f));
            lines.push_back(Line(-100.0f, 3.75f, 100.0f, 3.75f, 0.3f));
            
            // Add circles for lane markers
            circles.push_back(Circle(-50.0f, -3.75f, 1.0f, false));
            circles.push_back(Circle(0.0f, -3.75f, 1.0f, false));
            circles.push_back(Circle(50.0f, -3.75f, 1.0f, false));
            
			//z lewej do prawej
            travelPoints1 = {
                Vec2(-90.0f, 1.75f),
                Vec2(-90.0f, 1.75f),
                Vec2(-60.0f, 1.75f),
                Vec2(-30.0f, 1.75f),

                Vec2(-5.0f, 1.75f),
                Vec2(5.0f, 1.75f),

                Vec2(30.0f, 1.75f),
                Vec2(60.0f, 1.75f),
                Vec2(90.0f, 1.75f),
                Vec2(90.0f, 1.75f)
            };
            //z lewej w dol
            travels.push_back(Travel(travelPoints1, 2, 1));
            travelPoints2 = {
                Vec2(-90.0f, 1.75f),
                Vec2(-90.0f, 1.75f),
                Vec2(-60.0f, 1.75f),
                Vec2(-30.0f, 1.75f),

                Vec2(-5.0f, 1.75f),
                Vec2(-1.75f, 1.75f),
                Vec2(-1.75f, 5.0f),

                Vec2(-1.75f, 15.0f),
                Vec2(-1.75f, 30.0f),
                Vec2(-1.75f, 45.0f),
                Vec2(-1.75f, 45.0f)
            };
            //z prawej do lewej
            travels.push_back(Travel(travelPoints2, 2, 2));
            travelPoints3 = {
                Vec2(90.0f, -1.75f),
                Vec2(90.0f, -1.75f),
                Vec2(60.0f, -1.75f),
                Vec2(30.0f, -1.75f),
                Vec2(15.0f, -1.75f),

                Vec2(5.0f, -1.75f),
                Vec2(-5.0f, -1.75f),

                Vec2(-30.0f, -1.75f),
                Vec2(-60.0f, -1.75f),
                Vec2(-90.0f, -1.75f),
                Vec2(-90.0f, -1.75f)
            };
            travels.push_back(Travel(travelPoints3, 1, 3));
			break;
        case 2: //lewoskret jeden konflikt
			//gorny z prawej do lewej
            blocks.push_back(Block(3.5f, 0.0f, 4.0f, 3.5f));
            blocks.push_back(Block(3.5f, 0.0f, 100.0f, 0.2f));
            blocks.push_back(Block(3.5f, 0.0f, 23.5f, 0.5f));
            blocks.push_back(Block(-100.0f, -4.0f, 0.0f, -3.5f));
            blocks.push_back(Block(-100.0f, 3.5f, -40.0f, 4.0f));
            blocks.push_back(Block(-100.0f, 0.0f, 0.0f, 0.2f));
            blocks.push_back(Block(-40.0f, 3.5f, -39.5f, 7.0f));
            blocks.push_back(Block(-40.0f, 7.0f, 23.5f, 7.5f));
            blocks.push_back(Block(23.5f, 3.5f, 24.0f, 7.5f));
            blocks.push_back(Block(23.5f, 3.5f, 100.0f, 4.0f));
            blocks.push_back(Block(3.5f, -4.0f, 100.0f, -3.5f));
            blocks.push_back(Block(3.5f, -100.0f, 4.0f, -3.5f));
            blocks.push_back(Block(-0.5f, -100.0f, 0.0f, -3.5f));
            blocks.push_back(Block(-20.0f, 3.5f, 3.5f, 3.7f));
            
            // Add lines for junction markings
            lines.push_back(Line(-100.0f, -3.75f, 100.0f, -3.75f, 0.3f));
            lines.push_back(Line(-100.0f, 3.75f, 0.0f, 3.75f, 0.3f));
            lines.push_back(Line(0.0f, 3.75f, 23.5f, 7.25f, 0.3f));
            
            // Add circles for conflict zone markers
            circles.push_back(Circle(0.0f, 0.0f, 2.0f, false));
            circles.push_back(Circle(-20.0f, 3.6f, 1.5f, false));
            
            travelPoints1 = {
                Vec2(100.0f, -1.75f),
                Vec2(100.0f, -1.75f),
                Vec2(90.0f, -1.75f),
                Vec2(60.0f, -1.75f),
                Vec2(30.0f, -1.75f),
                Vec2(5.0f, -1.75f),
                 Vec2(-5.0f, -1.75f),

                Vec2(-30.0f, -1.75f),
                Vec2(-60.0f, -1.75f),
                Vec2(-90.0f, -1.75f),
                Vec2(-100.0f, -1.75f),
				Vec2(-100.0f, -1.75f)
            };
            //dolny z lewej do prawej
            travels.push_back(Travel(travelPoints1, 5, 1));
            travelPoints2 = {
                Vec2(-100.0f, 1.75f),
                Vec2(-100.0f, 1.75f),
                Vec2(-90.0f, 1.75f),
                Vec2(-60.0f, 1.75f),

                Vec2(-40.0f, 1.75f),
                Vec2(-35.0f, 1.75f),
                Vec2(-30.0f, 3.5f),
                Vec2(-25.0f, 5.25f),
                Vec2(-20.0f, 5.25f),

                Vec2(-5.0f, 5.25f),
                Vec2(3.5f, 5.25f),
                Vec2(8.5f, 5.25f),
                Vec2(13.5f, 3.5f),
                Vec2(18.5f, 1.75f),
                Vec2(23.5f, 1.75f),
                
                Vec2(40.0f, 1.75f),
                Vec2(60.0f, 1.75f),
                Vec2(90.0f, 1.75f),
                Vec2(100.0f, 1.75f),
                Vec2(100.0f, 1.75f)
            };
			//srodkowy z lewej w gore
            travels.push_back(Travel(travelPoints2, 5, 2));
            travelPoints3 = {
                Vec2(-90.0f, 1.75f),
                Vec2(-90.0f, 1.75f),
                Vec2(-60.0f, 1.75f),
                Vec2(-30.0f, 1.75f),
                //Vec2(-10.0f, 1.75f),

                Vec2(-1.5f, 1.75f),
                Vec2(1.75f, 1.75f),
                Vec2(1.75f, -1.5f),

                Vec2(1.75f, -30.0f),
                Vec2(1.75f, -60.0f),
                Vec2(1.75f, -90.0f),   
                Vec2(1.75f, -90.0f)
            };
            travels.push_back(Travel(travelPoints3, 1, 3));
			break;
		case 3: //pojedynczy pas, przejscie dla pieszych
            blocks.push_back(Block(-100.0f, -2.0f, 100.0f, -1.75f));
            blocks.push_back(Block(-100.0f, 1.75f, 100.0f, 2.0f));
            blocks.push_back(Block(0.0f, -1.75f, 5.0f, 1.75f, 10.0f, 5.0f));
            
            // Add lines for single lane
            lines.push_back(Line(-100.0f, 0.0f, 100.0f, 0.0f, 0.2f));
            
            // Add circles for pedestrian crossing markers
            for (float x = -90.0f; x <= 90.0f; x += 20.0f) {
                circles.push_back(Circle(x, -1.75f, 0.5f, false));
                circles.push_back(Circle(x, 1.75f, 0.5f, false));
            }
            
            //z lewej do prawej
            travelPoints1 = {
                Vec2(-100.0f, 0.0f),
                Vec2(-100.0f, 0.0f),
                Vec2(-60.0f, 0.0f),
                Vec2(-30.0f, 0.0f),
                Vec2(0.0f, 0.0f),
                Vec2(30.0f, 0.0f),
                Vec2(60.0f, 0.0f),
                Vec2(100.0f, 0.0f),
                Vec2(100.0f, 0.0f)
            };
            travels.push_back(Travel(travelPoints1, 1, 1));
        case 4: //proste rondo
            circles.push_back(Circle(0.0f, 0.0f, 11.0f, true));
            circles.push_back(Circle(0.0f, 0.0f, 17.0f, false));

            lines.push_back(Line(-150.0f, -3.5f, -35.0f, -3.5f, 0.2f));
            lines.push_back(Line(-150.0f, 0.0f, -25.0f, 0.0f, 0.2f));
            lines.push_back(Line(-150.0f, 3.5f, -35.0f, 3.5f, 0.2f));

            lines.push_back(Line(35.0f, -3.5f, 150.0f, -3.5f, 0.2f));
            lines.push_back(Line(25.0f, 0.0f, 150.0f, 0.0f, 0.2f));
            lines.push_back(Line(35.0f, 3.5f, 150.0f, 3.5f, 0.2f));

            lines.push_back(Line(-3.5f, -150.0f, -3.5f, -35.0f, 0.2f));
            lines.push_back(Line(0.0f, -150.0f, 0.0f, -25.0f, 0.2f));
            lines.push_back(Line(3.5f, -150.0f, 3.5f, -35.0f, 0.2f));

            lines.push_back(Line(-35.0f, -3.5f, -14.5f, -8.8f, 0.2f));
            lines.push_back(Line(-35.0f, 3.5f, -14.5f, 8.8f, 0.2f));
            lines.push_back(Line(-25.0f, 0.0f, -16.9f, -1.8f, 0.2f));
            lines.push_back(Line(-25.0f, 0.0f, -16.9f, 1.8f, 0.2f));

            lines.push_back(Line(35.0f, -3.5f, 14.5f, -8.8f, 0.2f));
            lines.push_back(Line(35.0f, 3.5f, 14.5f, 8.8f, 0.2f));
            lines.push_back(Line(25.0f, 0.0f, 16.9f, -1.8f, 0.2f));
            lines.push_back(Line(25.0f, 0.0f, 16.9f, 1.8f, 0.2f));

            lines.push_back(Line(-3.5f, -35.0f, -8.8f, -14.5f, 0.2f));
            lines.push_back(Line(3.5f, -35.0f, 8.8f, -14.5f, 0.2f));
            lines.push_back(Line(0.0f, -25.0f, -1.8f, -16.9f, 0.2f));
            lines.push_back(Line(0.0f, -25.0f, 1.8f, -16.9f, 0.2f));

            //z lewej do prawej
            travelPoints1 = {
                Vec2(-150.0f, 1.75f),
                Vec2(-150.0f, 1.75f),
                Vec2(-100.0f, 1.75f),
                Vec2(-60.0f, 1.75f),

                Vec2(-35.0f, 1.75f),
                Vec2(-19.0f, 2.0f),
                Vec2(-16.2f, 4.0f),
                Vec2(-7.0f, 14.0f),
                Vec2(0.0f, 14.0f),
                Vec2(8.0f, 14.0f),
                Vec2(13.0f, 7.5f),
                Vec2(19.00f, 2.0f),
                Vec2(35.0f, 1.75f),


                Vec2(40.0f, 1.75f),
                Vec2(60.0f, 1.75f),
                Vec2(80.0f, 1.75f),
                Vec2(100.0f, 1.75f),
                Vec2(150.0f, 1.75f),
                Vec2(150.0f, 1.75f),
            };
			

            //z lewej w gore
            travelPoints2 = {
                Vec2(-150.0f, 1.75f),
                Vec2(-150.0f, 1.75f),
                Vec2(-100.0f, 1.75f),
                Vec2(-60.0f, 1.75f),

                Vec2(-35.0f, 1.75f),
                Vec2(-19.0f, 2.0f),
                Vec2(-16.2f, 4.0f),
                Vec2(-7.0f, 14.0f),
                Vec2(0.0f, 14.0f),
                Vec2(8.0f, 14.0f),
                Vec2(12.8f, 7.25f),
                Vec2(16.0f, 0.f),
                Vec2(12.8f, -5.5f),
                //Vec2(12.8f, -7.25f),
                Vec2(10.5f, -9.0f),
                Vec2(7.0f, -12.0f),
                Vec2(1.5f, -17.0f),
                Vec2(1.75f, -35.0f),

                Vec2(1.75f, -60.0f),
                Vec2(1.75f, -90.0f),
                Vec2(1.75f, -120.0f),
                Vec2(1.75f, -150.0f),
                Vec2(1.75f, -150.0f),

            };
            //z prawej w gore
            travelPoints3 = {
                Vec2(150.0f, -1.75f),
                Vec2(150.0f, -1.75f),
                Vec2(120.0f, -1.75f),
                Vec2(100.0f, -1.75f),
                Vec2(90.0f, -1.75f),
                Vec2(60.0f, -1.75f),

                Vec2(35.0f, -1.75f),
                Vec2(22.0f, -2.5f),
                Vec2(16.2f, -6.0f),
                Vec2(1.75f, -16.0f),
                Vec2(1.75f, -35.0f),

                Vec2(1.75f, -60.0f),
                Vec2(1.75f, -90.0f),
                Vec2(1.75f, -120.0f),
                Vec2(1.75f, -150.0f),
                Vec2(1.75f, -150.0f),
            };
			//z prawej w lewo
            travelPoints4 = {
                Vec2(150.0f, -1.75f),
                Vec2(150.0f, -1.75f),
                Vec2(120.0f, -1.75f),
                Vec2(100.0f, -1.75f),
                Vec2(90.0f, -1.75f),
                Vec2(60.0f, -1.75f),

                Vec2(35.0f, -1.75f),
                Vec2(19.0f, -2.0f),
                Vec2(16.2f, -4.0f),
                Vec2(10.5f, -7.0f),
                Vec2(6.7f, -11.0f),
                Vec2(0.0f, -16.0f),
                Vec2(-8.8f, -9.2f),
                Vec2(-13.5f, -4.0f),
                Vec2(-35.0f, -1.75f),

                Vec2(-60.0f, -1.75f),
                Vec2(-90.0f, -1.75f),
                Vec2(-120.0f, -1.75f),
                Vec2(-150.0f, -1.75f),
                Vec2(-150.0f, -1.75f)
            };
			//z gory w lewo
            travelPoints5 = {
                Vec2(-1.75f, -150.0f),
                Vec2(-1.75f, -150.0f),
                Vec2(-1.75f, -120.0f),
                Vec2(-1.75f, -90.0f),
                Vec2(-1.75f, -60.0f),



                Vec2(-60.0f, -1.75f),
                Vec2(-90.0f, -1.75f),
                Vec2(-120.0f, -1.75f),
                Vec2(-150.0f, -1.75f),
                Vec2(-150.0f, -1.75f)
            };
			//z gory w prawo
            travelPoints6 = {
                Vec2(-1.75f, -150.0f),
                Vec2(-1.75f, -150.0f),
                Vec2(-1.75f, -120.0f),
                Vec2(-1.75f, -90.0f),
                Vec2(-1.75f, -60.0f),

                

                Vec2(40.0f, 1.75f),
                Vec2(60.0f, 1.75f),
                Vec2(80.0f, 1.75f),
                Vec2(100.0f, 1.75f),
                Vec2(150.0f, 1.75f),
                Vec2(150.0f, 1.75f),
            };
			travels.push_back(Travel(travelPoints1, 1, 1));
            travels.push_back(Travel(travelPoints2, 1, 2));
            travels.push_back(Travel(travelPoints3, 1, 3));
			travels.push_back(Travel(travelPoints4, 1, 4));
        default:
            std::cerr << "Unknown TJunction index: " << index << std::endl;
            break;
	}
/*    //for all travelPoints4 add small circle markers to indicate the path
    for (const Vec2& point : travelPoints4) {
        circles.push_back(Circle(point.x, point.y, 0.5f, false));
    }

    //for all travelPoints4 add small circle along bezier curve to indicate the path




    for (size_t i = 0; i + 2 < travelPoints4.size(); i += 2)
    {
        Vec2 p0 = travelPoints4[i];
        Vec2 p1 = travelPoints4[i + 1];
        Vec2 p2 = travelPoints4[i + 2];

        for (float t = 0.0f; t <= 1.0f; t += STEP)
        {
            Vec2 point = tempTravel.bezier(p0, p1, p2, t);

            circles.push_back(
                Circle(point.x, point.y, 0.3f, false)
            );
        }
    }*/
}

std::vector<Block> TJunction::getBlocks() {
    return blocks;
}

std::vector<Line> TJunction::getLines() {
    return lines;
}

std::vector<Circle> TJunction::getCircles() {
    return circles;
}

Travel TJunction::getRandomTravel() {
    int sum_of_weight = 0;
    for (int i = 0; i < travels.size(); i++) {
        sum_of_weight += travels[i].getWeight();
    }
    int rnd = rand() % sum_of_weight;
    for (int i = 0; i < travels.size(); i++) {
        if (rnd < travels[i].getWeight())
            return travels[i];
        rnd -= travels[i].getWeight();
    }
}
