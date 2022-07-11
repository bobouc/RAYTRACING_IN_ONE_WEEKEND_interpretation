#include "color.h"
#include "ray.h"
#include "vec3.h"

#include <iostream>

double hit_sphere(const point3& center, double radius, const ray& r) {
    vec3 oc = r.origin() - center;  // means A-C
    // t^2b⋅b+2b⋅(A−C)t+(A−C)⋅(A−C)−r^2=0 
    // a means t in code with equation.
    // b means 2b(A−C) in code with equation.
    // c means (A-c)⋅(A-C) -  r^2 in code with equation.
    auto a = dot(r.direction(), r.direction());
    auto b = 2.0 * dot(oc, r.direction());
    auto c = dot(oc, oc) - radius*radius;
    auto discriminant = b*b - 4*a*c;
    // return (discriminant > 0);
    if(discriminant < 0){
        return -1.0;
    }else{
        return (-b + sqrt(discriminant)) / (2.0*a);
    }
}
color ray_color(const ray& r) {
    auto t = hit_sphere(point3(0, 0, -1), 0.5, r);
    if(t > 0.0){
        vec3 N = unit_vector(r.at(t) - vec3(0, 0, -1));
        return 0.5*color(N.x()+1, N.y()+1, N.z()+1);
    }
    // unit vector
    vec3 unit_direction = unit_vector(r.direction());
    // mix parameter different with different position of direction's y.
    t = 0.5*(unit_direction.y() + 1.0);
    // color between blue and white.
    return (1.0-t)*color(1.0, 1.0, 1.0) + t*color(0.5, 0.7, 1.0);
}




int main() {

    // Image
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);

    // Camera

    auto viewport_height = 2.0;
    auto viewport_width = aspect_ratio * viewport_height;       // (16/9) * 2 = 3.5...
    auto focal_length = 1.0;

    auto origin = point3(0, 0, 0);
    auto horizontal = vec3(viewport_width, 0, 0);   //vec3(w_16:9, 0, 0)
    auto vertical = vec3(0, viewport_height, 0);
    // turn left and then turn down and then turn back to the Camera geometry.
    auto lower_left_corner = origin - horizontal/2 - vertical/2 - vec3(0, 0, focal_length);

    // Render

    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

    for (int j = image_height-1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            auto u = double(i) / (image_width-1);
            auto v = double(j) / (image_height-1);
            // calculate the ray of the pixel
            // std::cout<<u<<std::endl;
            ray r(origin, lower_left_corner + u*horizontal + v*vertical - origin);
            color pixel_color = ray_color(r);
            write_color(std::cout, pixel_color);
        }
    }

    std::cerr << "\nDone.\n";
}