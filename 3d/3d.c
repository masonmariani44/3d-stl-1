#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "3d.h"

Scene3D* Scene3D_create() {
    Scene3D * scene = malloc(sizeof(Scene3D));
    scene->count = 0;
    scene->root = NULL;
    return scene;
}

void Scene3D_destroy_helper(Triangle3DNode* node) {
    if (node == NULL) {
        return;
    }
    Scene3D_destroy_helper(node->next);
    free(node);
}

void Scene3D_destroy(Scene3D* scene) {
    Scene3D_destroy_helper(scene->root);
    free(scene);
}

void Scene3D_write_stl_text_helper(Triangle3DNode* node, FILE* stl_file) {
    if (node == NULL) {
        return;
    }
    fprintf(stl_file, "  facet normal 0.0 0.0 0.0\n    outer loop\n");
    Coordinate3D coord_a = node->triangle.a;
    Coordinate3D coord_b = node->triangle.b;
    Coordinate3D coord_c = node->triangle.c;
    fprintf(stl_file, "      vertex %.5f %.5f %.5f\n", coord_a.x, coord_a.y, coord_a.z);
    fprintf(stl_file, "      vertex %.5f %.5f %.5f\n", coord_b.x, coord_b.y, coord_b.z);
    fprintf(stl_file, "      vertex %.5f %.5f %.5f\n", coord_c.x, coord_c.y, coord_c.z);
    fprintf(stl_file, "    endloop\n  endfacet\n");
    Scene3D_write_stl_text_helper(node->next, stl_file);
}

void Scene3D_write_stl_text(Scene3D* scene, char* file_name) {
    FILE* stl_file;
	stl_file = fopen(file_name, "w");
    if (stl_file == NULL) {
        printf("error opening the file");   
        exit(1);             
    }

    fprintf(stl_file, "solid scene\n");
    Scene3D_write_stl_text_helper(scene->root, stl_file);
    fprintf(stl_file, "endsolid scene");

    fclose(stl_file);
}

void Scene3D_add_triangle(Scene3D* scene, Triangle3D triangle) {
    Triangle3DNode * new_triangle_node = malloc(sizeof(Triangle3DNode));
    new_triangle_node->triangle = triangle;
    new_triangle_node->next = NULL;

    if (scene->root == NULL) {
        scene->root = new_triangle_node;
    }
    else {
        //TODO very inefficient, very stupid, cant get it to work another way with offsets
        Triangle3DNode * cur = scene->root;
        while (cur->next != NULL) {
            cur = cur->next;
        }
        cur->next = new_triangle_node;
    }
    scene->count = scene->count+1;
}

void Scene3D_add_pyramid(Scene3D* scene, Coordinate3D origin, double width, double height, char* orientation) {
    double half_width = width / 2;

    Coordinate3D coord_a;
    Coordinate3D coord_b;
    Coordinate3D coord_c;
    Coordinate3D coord_d;
    Coordinate3D point;

    if (strcmp(orientation, "up") == 0 || strcmp(orientation, "down") == 0) {
        coord_a = (Coordinate3D) {origin.x-half_width, origin.y-half_width, origin.z};
        coord_b = (Coordinate3D) {origin.x-half_width, origin.y+half_width, origin.z};
        coord_c = (Coordinate3D) {origin.x+half_width, origin.y-half_width, origin.z};
        coord_d = (Coordinate3D) {origin.x+half_width, origin.y+half_width, origin.z};
        if (strcmp(orientation, "up") == 0) {
            point = (Coordinate3D) {origin.x, origin.y, origin.z+height};
        }
        else {
            point = (Coordinate3D) {origin.x, origin.y, origin.z-height};
        }
    }

    if (strcmp(orientation, "left") == 0 || strcmp(orientation, "right") == 0) {
        coord_a = (Coordinate3D) {origin.x, origin.y-half_width, origin.z-half_width};
        coord_b = (Coordinate3D) {origin.x, origin.y-half_width, origin.z+half_width};
        coord_c = (Coordinate3D) {origin.x, origin.y+half_width, origin.z-half_width};
        coord_d = (Coordinate3D) {origin.x, origin.y+half_width, origin.z+half_width};
        if (strcmp(orientation, "right") == 0) {
            point = (Coordinate3D) {origin.x+height, origin.y, origin.z};
        }
        else {
            point = (Coordinate3D) {origin.x-height, origin.y, origin.z};
        }
    }

    if (strcmp(orientation, "forward") == 0 || strcmp(orientation, "backward") == 0) {
        coord_a = (Coordinate3D) {origin.x-half_width, origin.y, origin.z-half_width};
        coord_b = (Coordinate3D) {origin.x-half_width, origin.y, origin.z+half_width};
        coord_c = (Coordinate3D) {origin.x+half_width, origin.y, origin.z-half_width};
        coord_d = (Coordinate3D) {origin.x+half_width, origin.y, origin.z+half_width};
        if (strcmp(orientation, "forward") == 0) {
            point = (Coordinate3D) {origin.x, origin.y+height, origin.z};
        }
        else {
            point = (Coordinate3D) {origin.x, origin.y-height, origin.z};
        }
    }

    //add shapes to scene
    Scene3D_add_quadrilateral(scene, coord_a, coord_b, coord_c, coord_d);
    Scene3D_add_triangle(scene, (Triangle3D) {coord_a, coord_b, point});
    Scene3D_add_triangle(scene, (Triangle3D) {coord_b, coord_d, point});
    Scene3D_add_triangle(scene, (Triangle3D) {coord_d, coord_c, point});
    Scene3D_add_triangle(scene, (Triangle3D) {coord_c, coord_a, point});
}

void Scene3D_add_cuboid(Scene3D* scene, Coordinate3D origin, double width, double height, double depth) {
    double half_width = width / 2;
    double half_height = height / 2;
    double half_depth = depth / 2;
    //top / bottom
    Coordinate3D coord_a = (Coordinate3D) {origin.x-half_width, origin.y-half_height, origin.z+half_depth};
    Coordinate3D coord_b = (Coordinate3D) {origin.x-half_width, origin.y+half_height, origin.z+half_depth};
    Coordinate3D coord_c = (Coordinate3D) {origin.x+half_width, origin.y-half_height, origin.z+half_depth};
    Coordinate3D coord_d = (Coordinate3D) {origin.x+half_width, origin.y+half_height, origin.z+half_depth};
    Scene3D_add_quadrilateral(scene, coord_a, coord_b, coord_c, coord_d);
    coord_a.z = coord_a.z - depth;
    coord_b.z = coord_b.z - depth;
    coord_c.z = coord_c.z - depth;
    coord_d.z = coord_d.z - depth;
    Scene3D_add_quadrilateral(scene, coord_a, coord_b, coord_c, coord_d);

    //left / right
    coord_a = (Coordinate3D) {origin.x+half_width, origin.y-half_height, origin.z-half_depth};
    coord_b = (Coordinate3D) {origin.x+half_width, origin.y+half_height, origin.z-half_depth};
    coord_c = (Coordinate3D) {origin.x+half_width, origin.y-half_height, origin.z+half_depth};
    coord_d = (Coordinate3D) {origin.x+half_width, origin.y+half_height, origin.z+half_depth};
    Scene3D_add_quadrilateral(scene, coord_a, coord_b, coord_c, coord_d);
    coord_a.x = coord_a.x - width;
    coord_b.x = coord_b.x - width;
    coord_c.x = coord_c.x - width;
    coord_d.x = coord_d.x - width;
    Scene3D_add_quadrilateral(scene, coord_a, coord_b, coord_c, coord_d);

    //front / back
    coord_a = (Coordinate3D) {origin.x+half_width, origin.y+half_height, origin.z-half_depth};
    coord_b = (Coordinate3D) {origin.x-half_width, origin.y+half_height, origin.z-half_depth};
    coord_c = (Coordinate3D) {origin.x+half_width, origin.y+half_height, origin.z+half_depth};
    coord_d = (Coordinate3D) {origin.x-half_width, origin.y+half_height, origin.z+half_depth};
    Scene3D_add_quadrilateral(scene, coord_a, coord_b, coord_c, coord_d);
    coord_a.y = coord_a.y - height;
    coord_b.y = coord_b.y - height;
    coord_c.y = coord_c.y - height;
    coord_d.y = coord_d.y - height;
    Scene3D_add_quadrilateral(scene, coord_a, coord_b, coord_c, coord_d);
}

void Scene3D_add_quadrilateral(Scene3D* scene, Coordinate3D a, Coordinate3D b, Coordinate3D c, Coordinate3D d) {
    Triangle3D triangle_1 = (Triangle3D) {a, b, c};
    Triangle3D triangle_2 = (Triangle3D) {b, c, d};
    Triangle3D triangle_3 = (Triangle3D) {a, c, d};
    Triangle3D triangle_4 = (Triangle3D) {a, b, d};

    Scene3D_add_triangle(scene, triangle_1);
    Scene3D_add_triangle(scene, triangle_2);
    Scene3D_add_triangle(scene, triangle_3);
    Scene3D_add_triangle(scene, triangle_4);
}