//
// Created by bahuljain on 10/22/16.
//

#ifndef RAYTRA_PROGRESSBAR_H
#define RAYTRA_PROGRESSBAR_H

#include <iostream>

using namespace std;


class ProgressBar {
public:
    ProgressBar() {};

    void start() {
        cout << "[";
    }

    void log(float completed, float total) {
        static int progress = 0;
        float percent_completed = completed / total * 100;

        if (percent_completed - progress >= 4) {
            progress += 4;

            if (progress % 20 == 0 && progress < 100)
                cout << flush << "=|" << progress << "%|";
            else
                cout << flush << "=";
        }
    }

    void done() {
        cout << "] \xF0\x9F\x8D\xBA \xF0\x9F\x8D\xBA " << endl;
    }
};


#endif //RAYTRA_PROGRESSBAR_H
