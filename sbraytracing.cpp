#include "sbraytracing.h"


bool Sphere::rayCast(const SBVector3 &orig, const SBVector3 &dir, float *length) {
    /*float length = - ((orig - center) * dir);
    SBVector3 centerPoint = orig + (dir * length);
    float radLen = (centerPoint - center).length();
    float radLenSq = radLen * radLen;
    qDebug() << radLen;
    if (radLenSq > radius * radius) return false;

    float lenToCen = sqrt(radius * radius - radLenSq);

    if (length >= lenToCen) {
        *intersect = centerPoint - (dir * lenToCen);
        return true;
    }

    if (length + lenToCen > 0) {
        *intersect = centerPoint + (dir * lenToCen);
        return true;
    }

    return false;*/
    SBVector3 L = center - orig;
    float tca = L * dir;
    float d2 = L * L - tca*tca;
    if (d2 > radius * radius) return false;
    float thc = sqrtf(radius * radius - d2);
    *length = tca - thc;
    float t1 = tca + thc;
    if (*length < 0) {
        *length = t1;
        return false;
    }
    return true;
}

bool castRay(Scene &scene, const SBVector3 &orig, const SBVector3 &dir, Material *mat, int depth, int cur_depth) {
    float intLen = std::numeric_limits<float>::max();
    float currLen = intLen;
    SBVector3 hit, normal, light_path, light_reflect_path;
    Material tmpm, reflectm;
    float light_intense = 0.0f;
    float light_specular_intense = 0.0f;
    size_t index = 0;
    for (size_t i = 0; i < scene.spheres.size(); i++) {
        if (scene.spheres[i].rayCast(orig, dir, &intLen)) {
            if (intLen < currLen) {
                currLen = intLen;
                index = i;
            }
        }
    }

    if (currLen < 1000) {
        light_intense = 0.0f;
        hit = orig + (dir * currLen);
        normal = (hit - scene.spheres[index].center).normalize();
        *mat = scene.spheres[index].mat;
        // reflections
        SBVector3 reflect_dir = reflectRay(dir, normal);
        SBVector3 reflect_orig = hit + (reflect_dir * 1e-3f);
        bool intersect_res;
        reflectm.diffuse_color = bg_color;
        if (cur_depth < depth) {
            intersect_res = castRay(scene, reflect_orig, reflect_dir, &reflectm, 0, 0);
        }

        //if (castRay(scene, shadow_orig, shadow_dir, &tmpm))
        for (size_t j = 0; j < scene.lights.size(); j++) {
            light_path = (scene.lights[j].position - hit).normalize();
            // shadows
            SBVector3 shadow_orig = hit + (light_path * 1e-3f);
            SBVector3 shadow_dir = light_path.normalize();
            if (castRay(scene, shadow_orig, shadow_dir, &tmpm, 0, 0))
                continue;
            // light
            light_intense += std::max(0.0f, light_path * normal) * scene.lights[j].bright;
            //qDebug() << scene.lights[j].position.getX() << scene.lights[j].position.getY() << scene.lights[j].position.getZ();
            light_reflect_path = reflectRay(light_path, normal);
            light_specular_intense += powf(std::max(0.0f, light_reflect_path * dir), mat->spec_exponent) * scene.lights[j].bright;
        }
        mat->diffuse_color = mix(mat->diffuse_color, reflectm.diffuse_color, mat->reflection);
        mat->diffuse_color = mat->diffuse_color * light_intense * mat->albedo.getX();
        mat->diffuse_color = mat->diffuse_color + SBVector3(1.0f, 1.0f, 1.0f) * light_specular_intense * mat->albedo.getY();

        return true;
    }
    else {
        mat->diffuse_color = bg_color;
        return false;
    }
}

SBVector3 reflectRay(const SBVector3 &dir, SBVector3 &normal) {
    return dir - normal * 2.0f * (normal * dir);
}

SBVector3 mix(const SBVector3 &v1, const SBVector3 &v2, float factor) {
    float factor2 = 1.0f - factor;
    return (v1 * factor2) + (v2 * factor);
}

void render(bool camera_type, Scene &scene, int antialiasing) {
    const int screen_width = 800, screen_height = 600;
    int width = screen_width * antialiasing;
    int height = screen_height * antialiasing;
    const float fov = M_PI / 3.0f;
    SBVector3 *framebuffer = new SBVector3[width * height];
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            framebuffer[x + width * y] = bg_color;//SBVector3(x/(float)width, y/(float)height, 0.0f);
        }
    }

    Material mat;
    SBVector3 dir;
    SBVector3 origVec;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (camera_type) {
                float xDir =  (2*(x + 0.5f)/(float)width  - 1)*tan(fov/2.0f)*width/(float)height;
                float yDir = -(2*(y + 0.5f)/(float)height - 1)*tan(fov/2.0f);
                dir = SBVector3(xDir, yDir, 1.0f).normalize();
                origVec = SBVector3(0.0f, 0.5f, -8.0f);
            }
            else {
                dir = SBVector3(0.0f, 0.0f, 1.0f);
                origVec = SBVector3((x - width / 2.0f) / 10.0f, (height / 2.0f - y) / 10.f , 0);
            }
            if (castRay(scene, origVec, dir, &mat, 4, 0))
                framebuffer[x + y * width] = mat.diffuse_color;
        }
    }

    SBVector3 *imagebuf = new SBVector3[screen_width * screen_height];

    // antialiasing SSAA x2 x4
    SBVector3 color_buf;
    for (int y = 0; y < screen_height; y++) {
        for (int x = 0; x < screen_width; x++) {
            color_buf = SBVector3(0.0f, 0.0f, 0.0f);
            for (int my = y * antialiasing; my < (y + 1) * antialiasing; my++) {
                for (int mx = x * antialiasing; mx < (x + 1) * antialiasing; mx++) {
                    color_buf = color_buf + framebuffer[mx + width * my];
                }
            }
            imagebuf[x + screen_width * y] = (color_buf / (antialiasing * antialiasing));
        }
    }

#ifndef QT_SAVE
    std::ofstream ofile;
    ofile.open("D:/geometry_output.ppm");
    ofile << "P3 " << screen_width << " " << screen_height << " 255 ";
    for (int y = 0; y < screen_height; y++) {
        for (int x = 0; x < screen_width; x++) {
            ofile << std::to_string((int)(255 * std::max(0.0f, std::min(1.0f, imagebuf[x + screen_width * y].getX())))) << " ";
            ofile << std::to_string((int)(255 * std::max(0.0f, std::min(1.0f, imagebuf[x + screen_width * y].getY())))) << " ";
            ofile << std::to_string((int)(255 * std::max(0.0f, std::min(1.0f, imagebuf[x + screen_width * y].getZ())))) << " ";
        }
    }
    ofile.close();
#else
    QImage image(QSize(screen_width, screen_height), QImage::Format_RGB888);
    for (int y = 0; y < screen_height; y++) {
        for (int x = 0; x < screen_width; x++) {
            image.setPixelColor(x, y, QColor(
            (int)(255 * std::max(0.0f, std::min(1.0f, imagebuf[x + screen_width * y].getX()))),
            (int)(255 * std::max(0.0f, std::min(1.0f, imagebuf[x + screen_width * y].getY()))),
            (int)(255 * std::max(0.0f, std::min(1.0f, imagebuf[x + screen_width * y].getZ())))
            ));
        }
    }
    image.save("D:/geometry_output.png");
#endif
    delete [] framebuffer;
    delete [] imagebuf;
}
