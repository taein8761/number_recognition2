#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;
Mat mat;
void onMouse(int event, int x, int y, int flags, void* userdata);
void drawUI(Mat& mat);
void saveimage();
void loadimage();
void clearimage();
void exitProgram();
void feature1();
int main(void) {
    mat = Mat(500, 900, CV_8UC3, Scalar(255, 255, 255));
    drawUI(mat);

    namedWindow("mat");
    setMouseCallback("mat", onMouse);
    imshow("mat", mat);
    waitKey(0);

    return 0;
}
void drawUI(Mat& mat) {
    rectangle(mat, Rect(0, 0, 699, 499), Scalar(0, 0, 0), 2);
    string cha[] = { "Save", "Load", "Clear", "Run", "Exit" ,"feature1" };
    for (int i = 0; i < 5; i++) {
        rectangle(mat, Rect(500, i * 100, 200, 100), Scalar(0, 0, 0), 2);
        rectangle(mat, Rect(700, i * 100, 200, 100), Scalar(0, 0, 0), 2);
        putText(mat, cha[i], Point(530, 65 + i * 100), FONT_HERSHEY_SIMPLEX, 2, Scalar(0, 0, 0), 3);
    }
    putText(mat, cha[5], Point(720, 65 + 0), FONT_HERSHEY_SIMPLEX, 1.3, Scalar(0, 0, 0), 2.5);
}
void onMouse(int event, int x, int y, int flags, void* userdata) {
    static Point ptOld;
    Rect save_rect(500, 0, 200, 100);
    Rect load_rect(500, 100, 200, 100);
    Rect clear_rect(500, 200, 200, 100);
    Rect run_rect(500, 300, 200, 100);
    Rect exit_rect(500, 400, 200, 100);
    Rect feature1_rect(700, 0, 200, 100);
    if (x >= 0 && x < 500) {
        switch (event) {
        case EVENT_LBUTTONDOWN:
            ptOld = Point(x, y);
            break;
        case EVENT_LBUTTONUP:
            break;
        case EVENT_MOUSEMOVE:
            if (flags & EVENT_FLAG_LBUTTON) {
                line(mat, ptOld, Point(x, y), Scalar(0, 0, 0), 20);
                imshow("mat", mat);
                ptOld = Point(x, y);
            }
            break;
        }
    }
    if (event == EVENT_LBUTTONDOWN) {
        if (save_rect.contains(Point(x, y))) {
            saveimage();
        }
        else if (load_rect.contains(Point(x, y))) {
            loadimage();
        }
        else if (clear_rect.contains(Point(x, y))) {
            clearimage();
        }
        else if (run_rect.contains(Point(x, y))) {
            cout << "Run 버튼 클릭됨" << endl;
        }
        else if (exit_rect.contains(Point(x, y))) {
            exitProgram();
        }
        else if (feature1_rect.contains(Point(x, y))) {
            feature1();
        }
    }
}
void saveimage() {
    string save_name;
    cout << "파일명 입력:";
    cin >> save_name;
    Mat mat500 = mat(Rect(2, 2, 496, 496));
    resize(mat500, mat500, Size(500, 500));
    imwrite(save_name, mat500);
    cout << save_name << "파일이 저장됨" << endl;
}
void loadimage() {
    string load_name;
    cout << "파일명 입력:";
    cin >> load_name;
    Mat load_mat = imread(load_name, IMREAD_COLOR);
    if (load_mat.empty()) {
        cout << "파일을 불러오는데 실패했습니다: " << load_name << endl;
    }
    else {
        cout << load_name << " 파일을 불러옴" << endl;
        Rect load_rect(Point(0, 0), load_mat.size());
        load_mat.copyTo(mat(load_rect));
        drawUI(mat); // UI 다시 그리기
        imshow("mat", mat);
    }
}
void clearimage() {
    cout << "입력창 삭제됨" << endl;
    mat = Mat(500, 900, CV_8UC3, Scalar(255, 255, 255));
    drawUI(mat);
    imshow("mat", mat);
}
void exitProgram() {
    cout << "프로그램 종료" << endl;
    destroyAllWindows();
}
void feature1() {
    Mat feature1;
    // 검은색 테두리 부분 제외
    feature1 = mat(Rect(2, 2, 496, 496));

    Mat gray;
    // 그레이 영상 변경
    cvtColor(feature1, gray, COLOR_BGR2GRAY);

    Mat bin;
    // 이진화
    threshold(gray, bin, 0, 255, THRESH_BINARY_INV);

    Mat labels, stats, centroids;
    // labels = 레이블 맵 행렬, stats = CV_.., cen = 무게 중심 좌표
    int cnt = connectedComponentsWithStats(bin, labels, stats, centroids);

    cvtColor(feature1, feature1, COLOR_BGR2BGRA); // BGRA로 변환
    
    for (int i = 1; i < cnt; i++) {
        int* p = stats.ptr<int>(i); // stats 객체의 i행의 시작주소를 리턴
        if (p[4] < 20) continue;    // 면적 필터링 (p[4]는 면적)
        rectangle(feature1, Rect(p[0], p[1], p[2], p[3]), Scalar(0,0,255)); // 윤곽선 그리기
    }

    imshow("feature1", feature1);
}

