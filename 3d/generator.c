#include <stdlib.h>
#include <stdio.h>
#include "3d.h"

int main() {
    char* file_name;
    file_name = "output.stl";

    //🗿
    //incredible
    Scene3D* scene = Scene3D_create();
    Coordinate3D coordinate;
    // Head
    coordinate = (Coordinate3D){25, 25, 25};
    Scene3D_add_cuboid(scene, coordinate, 30, 30, 50);
    coordinate = (Coordinate3D){25, 25, 0};
    Scene3D_add_pyramid(scene, coordinate, 35, 71, "up");
    // brow
    coordinate = (Coordinate3D){25, 22, 60};
    Scene3D_add_cuboid(scene, coordinate, 30, 36, 22);
    // Mouth
    coordinate = (Coordinate3D){25, 9, 20};
    Scene3D_add_cuboid(scene, coordinate, 23, 2, 3);
    coordinate = (Coordinate3D){25, 9, 16};
    Scene3D_add_cuboid(scene, coordinate, 23, 2, 3);
    coordinate = (Coordinate3D){36, 9, 17};
    Scene3D_add_cuboid(scene, coordinate, 4, 2, 3);
    coordinate = (Coordinate3D){14, 9, 17};
    Scene3D_add_cuboid(scene, coordinate, 4, 2, 3);
    coordinate = (Coordinate3D){25, 9, 18};
    Scene3D_add_cuboid(scene, coordinate, 25, 1, 3);
    //nose
    coordinate = (Coordinate3D){25, 13, 24};
    Scene3D_add_pyramid(scene, coordinate, 22, 35, "up");
    //ears
    coordinate = (Coordinate3D){41, 25, 40};
    Scene3D_add_cuboid(scene, coordinate, 2, 15, 40);
    coordinate = (Coordinate3D){9, 25, 40};
    Scene3D_add_cuboid(scene, coordinate, 2, 15, 40);

    Scene3D_write_stl_text(scene, file_name);
	Scene3D_destroy(scene);
}