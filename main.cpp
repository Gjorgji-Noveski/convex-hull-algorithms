#include <iostream>
#include <vector>
#include <stack>
#include <ctime>
#include <algorithm>
#include <math.h>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;
int range;
class Point{
    public:
        float x;
        float y;
        Point(){}
        Point(int point_range){
            x = rand()%point_range + 1 *1.0;
            y = rand()%point_range + 1 *1.0;
        }

        Point (double x, double y){
            this->x = x;
            this->y = y;
        }
        void print(){
            printf("Point x:%f, y:%f\n", x, y);
        }
        friend bool operator==(const Point& p1, const Point& p2)
    {
        return ((p1.x == p2.x) && (p1.y == p2.y));
    }

};

class GrahamScan{
private:


public:
    std::vector<Point> points;

    void create_points(int number_of_points){
        for(int i=0; i<number_of_points; i++){
            Point a = Point(range);
            points.push_back(a);
        }
    }

    Point find_lowest_point(){
        Point lowest = points[0];
        if (points.size() > 1){

            for(int i=1; i<points.size(); i++){
                if(points[i].y < lowest.y){
                    lowest = points[i];
                    point_swap(points[i], points[0]);
                    }
                else if (points[i].y == lowest.y && points[i].x < lowest.x){
                        lowest = points[i];
                        point_swap(points[i], points[0]);
                }
        }
        }
        return lowest;
    }

    void print_points(){
        for(int i=0; i<points.size();i++){
            points[i].print();
        }
    }

    static double calculate_angle(Point &a, Point& b){
        double angle = atan2(b.y - a.y, b.x - a.x) * 180 / 3.1415926535897;
        if (angle < 0) angle+=360;
        return angle;
    }
    static double calculate_angle_3_points(Point &a, Point& b, Point &c){
        double angle1 = calculate_angle(a,b);
        double angle2 = calculate_angle(b,c);
        double angle = angle2 - angle1;
        return angle;
    }
    void point_swap(Point &p1, Point &p2){
    Point temp = p1;
    p1 = p2;
    p2 = temp;
}
    static int orientation(Point &a, Point &b, Point &c){
        double angle1 = calculate_angle(a,b);
        double angle2 = calculate_angle(b,c);
        double angle = angle2 - angle1;
        // 0 collinear, 1 counter clock-wise, -1 clock-wise
        if (angle > 0) return 1;
        if (angle < 0) return -1;
        return 0;

    }
};

    double distSq(Point p1, Point p2)
{
    return (p1.x - p2.x)*(p1.x - p2.x) +
          (p1.y - p2.y)*(p1.y - p2.y);
}
unsigned long int t1,t2,t3,t4;
ofstream graham_time_file;
ofstream jarvis_time_file;
ofstream graham_hull_file;
ofstream jarvis_hull_file;

void execute_graham(int n, bool save_convex_points){
    t1 = time(NULL);
    GrahamScan gs = GrahamScan();
    gs.create_points(n);
    Point lowest = gs.find_lowest_point();
    std::vector<std::pair<Point, double>> points_and_angles;

    for(int i =1; i<gs.points.size(); i++){
        std::pair<Point, double> point_angle = std::make_pair(gs.points[i], GrahamScan::calculate_angle(lowest, gs.points[i]));
        points_and_angles.push_back(point_angle);
    }

    std::sort(points_and_angles.begin(), points_and_angles.end(),
              [&lowest]
              (std::pair<Point, double> a, std::pair<Point, double> b){

              if (a.second != b.second) return a.second < b.second;
                else if (a.second == b.second){

                    //check above
                    if (lowest.x == a.first.x && lowest.x == b.first.x){
                        return a.first.y > b.first.y;
                }
                    //check right quadrant
                    if (lowest.x <= a.first.x && lowest.x <= b.first.x){
                        return a.first.x < b.first.x;
                }

                    //check left quadrant
                    if (lowest.x >= a.first.x && lowest.x >= b.first.x){
                        return a.first.x < b.first.x;
                }
                }
              }
              );

    for(int i = points_and_angles.size()-1; i>=0; --i){
            if (points_and_angles[i].first == points_and_angles[i-1].first){
                points_and_angles.erase(points_and_angles.begin()+i);
            }
        }

    t2 = time(NULL);
    t3 = time(NULL);
    std::stack<std::pair<Point, double>, std::vector<std::pair<Point, double>>> hull_stack;
    std::pair<Point, double> * second = &points_and_angles[0];
    std::pair<Point, double> * last = &points_and_angles[points_and_angles.size()-1];

    // lowest in hull
    hull_stack.push(std::make_pair(lowest, 0.0));

    //second in hull
    hull_stack.push(*second);

    std::pair<Point, double> * previous_point = &hull_stack.top();
    std::pair<Point, double> * current = &points_and_angles[1];
    std::pair<Point, double> * next = &points_and_angles[2];
    while(current != last){
        int orientation =
        GrahamScan::orientation(previous_point->first, current->first, next->first);
            if (orientation != -1){
                hull_stack.push(*current);
                current = &hull_stack.top();

                previous_point = current;
                current = next;
                next = ++next;
            }
            else{
                current = previous_point;
                previous_point = --previous_point;

                hull_stack.pop();
            }
        }
    hull_stack.push(*last);
    hull_stack.push(std::make_pair(lowest, 0.0));
    t4 = time(NULL);
    // Writing answers to file
    if(save_convex_points){
        graham_hull_file.open("graham_hull.txt");
        graham_hull_file << gs.points.size() << endl;
        for(int i = 0; i < gs.points.size(); i++){
            graham_hull_file << gs.points[i].x << ',' << gs.points[i].y <<endl;
        }
        Point p;
        while(!hull_stack.empty()){
        p = hull_stack.top().first;
        graham_hull_file << p.x << ',' << p.y << endl;
        hull_stack.pop();
    }
    graham_hull_file.close();
    }
}
void execute_jarvis(int n, bool save_convex_points){

    t1 = time(NULL);

    GrahamScan gs = GrahamScan();
    gs.create_points(n);
    Point lowest = gs.find_lowest_point();

    std::vector<std::pair<Point, double>> points_and_angles;

    for(int i =1; i<gs.points.size(); i++){
        std::pair<Point, double> point_angle = std::make_pair(gs.points[i], GrahamScan::calculate_angle(lowest, gs.points[i]));
        points_and_angles.push_back(point_angle);
    }


    t2=time(NULL);
    std::sort(points_and_angles.begin(), points_and_angles.end(),
              [&lowest]
              (std::pair<Point, double> a, std::pair<Point, double> b){

              if (a.second != b.second) return a.second < b.second;
                else if (a.second == b.second){

                    //check above
                    if (lowest.x == a.first.x && lowest.x == b.first.x){
                        return a.first.y > b.first.y;
                }
                    //check right quadrant
                    if (lowest.x <= a.first.x && lowest.x <= b.first.x){
                        return a.first.x < b.first.x;
                }

                    //check left quadrant
                    if (lowest.x >= a.first.x && lowest.x >= b.first.x){
                        return a.first.x < b.first.x;
                }
                }
              }
              );

    t3 = time(NULL);
    for(int i = points_and_angles.size()-1; i>=0; --i){
            if (points_and_angles[i].first == points_and_angles[i-1].first){
                points_and_angles.erase(points_and_angles.begin()+i);
            }
        }

    Point *tmp = nullptr;
    Point *pivot = nullptr;
    Point *prev = nullptr;
    double smallest_angle = 360;
    int orientation;
    std::vector<Point> copied_points;
    copied_points.push_back(lowest);

    for(int i =0; i<points_and_angles.size(); i++){
        copied_points.push_back(points_and_angles[i].first);
    }
    copied_points.push_back(lowest);
    std::stack<Point> answer_stack;

    answer_stack.push(lowest);
    prev = &answer_stack.top();
    pivot = &answer_stack.top();

    double angle;
    double anglet;
    Point * candidate_pivot = nullptr;
    int position_of_vector_to_be_deleted;

    while(true){
        position_of_vector_to_be_deleted = -1;
        smallest_angle = 360;
        candidate_pivot = nullptr;
            for(int k = 0; k < copied_points.size(); k++){

                tmp = &copied_points[k];
                if (*tmp == *pivot)continue;
                angle = GrahamScan::calculate_angle_3_points(*prev, *pivot, *tmp);
                orientation = GrahamScan::orientation(*prev, *pivot, *tmp);
                    if (angle < smallest_angle && orientation != -1){
                        smallest_angle = angle;
                        candidate_pivot = tmp;
                        position_of_vector_to_be_deleted = k;
                    }
        }
        prev = pivot;
        answer_stack.push(*candidate_pivot);
        pivot = &answer_stack.top();

        if (*pivot == lowest) break;

        copied_points.erase(copied_points.begin()+position_of_vector_to_be_deleted);
    }
    t4 = time(NULL);
    // Writing answers to file

    if(save_convex_points){
        jarvis_hull_file.open("jarvis_hull.txt");
        jarvis_hull_file << gs.points.size() << endl;
        for(int i = 0; i < gs.points.size(); i++){
            jarvis_hull_file << gs.points[i].x << ',' << gs.points[i].y <<endl;
        }
        Point pp;
        while (answer_stack.size() > 0){
            pp = answer_stack.top();
            jarvis_hull_file << pp.x << ',' << pp.y << endl;
            answer_stack.pop();
        }
        jarvis_hull_file.close();
    }
}

int main()
{
    srand (static_cast <unsigned> (time(0)));
    string graham_timefile_location = "graham_time.txt";
    string jarvis_timefile_location = "jarvis_time.txt";

    int number_of_points;
    int end_after_n_iterations;
    int i = 0;
    int algorithm; // 1 = Graham Scan, 2 = Gift Wrapping (Jarvis March)
    void (*algo_ptr)(int,bool);
    int run_type;
    int step_size;
    std::cout<<"Hello, do you want to run the algorithms just once or perform a time evaluation?"<<endl<<"Type 1 for a one-time run or 2 for performing evaluation"<<endl;

    while(true){
        std::cin>>run_type;
        if(!std::cin) {std::cin.clear(); cin.ignore(100000, '\n');}
        if(run_type == 1 || run_type == 2) break;
        std::cout<<"Invalid input detected, try again"<<endl;

    }

    std::cout<<"Which algorithm do you choose?"<<endl<<"Type 1 for Graham Scan or 2 for Gift Wrapping(Jarvis March)"<<endl;

    while(true){
        std::cin>>algorithm;
        if(!std::cin) {std::cin.clear(); cin.ignore(100000, '\n');}
        if(algorithm == 1 || algorithm == 2)break;
        std::cout<<"Invalid input detected, try again"<<endl;

    }

    if(run_type == 1){
        while(true){
        std::cout<<"Enter how many points should be randomly generated (must be more than 2)"<<endl;
        std::cin>>number_of_points;
        if(!std::cin || number_of_points < 3) {std::cin.clear(); cin.ignore(100000, '\n');}
        else break;
        std::cout<<"Invalid input detected, try again"<<endl;
        }

    }
    else if (run_type == 2){
        while(true){
        std::cout<<"Enter the starting number of points of the evaluation"<<endl;
        std::cin>>number_of_points;
        std::cout<<"Enter the step size"<<endl;
        std::cin>>step_size;
        std::cout<<"Stop after how many iterations?"<<endl;
        std::cin>>end_after_n_iterations;
        if(!std::cin) {std::cin.clear(); cin.ignore(100000, '\n');}
        else break;
        std::cout<<"Invalid input detected, try again"<<endl;
        }
    }

    if (algorithm == 1){
        algo_ptr = &execute_graham;
    }
    else if (algorithm == 2){
        algo_ptr = &execute_jarvis;
    }

    if(run_type == 1){
        printf("Running once with %d points\n", number_of_points);
        range = number_of_points;
        algo_ptr(number_of_points, true);
    }
    else if (run_type == 2){
        if (algorithm == 1) graham_time_file.open(graham_timefile_location);
        else if (algorithm == 2) jarvis_time_file.open(jarvis_timefile_location);
        while(true){
            if (end_after_n_iterations == i) break;
            printf("Running with %d points\n", number_of_points);
            range = number_of_points;
            algo_ptr(number_of_points, false);
            cout<<"Time taken:"<<(t2-t1) + (t4-t3)<<"s"<<endl;
            if (algorithm == 1) graham_time_file<<number_of_points<<','<<(t2-t1) + (t4-t3)<<endl;
            else if (algorithm == 2) jarvis_time_file<<number_of_points<<','<<(t2-t1) + (t4-t3)<<endl;
            ++i;
            number_of_points+=step_size;
        }
    }

    if (algorithm == 1) graham_time_file.close();
    else if (algorithm == 2) jarvis_time_file.close();
    return 0;
}
