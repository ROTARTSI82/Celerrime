#include "cel/game/dbg/mandelbrot_layer.hpp"

#include <cel/eng/misc_util.hpp>
#include <cel/app.hpp>

#include <imgui/imgui.h>
#include <fmt/format.h>

namespace cel::dbg {
    mandelbrot_layer::mandelbrot_layer(world_t &w) : test(4096, w.app->qvbo) {
        shader frag{GL_FRAGMENT_SHADER, read_entire_file(w.app->opt.res_path("frag/newton_fractal.frag"))};
        shader vert{GL_VERTEX_SHADER, read_entire_file(w.app->opt.res_path("vert/default.vert"))};

        shaders.attach(frag.get_id());
        shaders.attach(vert.get_id()); 
        shaders.link();

        // test.instances[0] = draw_instance{gl_mat2::transform({3.5, 2}, consts_for<float>::pi()), {0, 0, -5}, 1, {-2.5, -1}, {3.5, 2}, {1, 1}};
        test.instances[0] = draw_instance{gl_mat2::transform({3.5, 2}, consts_for<float>::pi()), {0, 0, -5}, 1, {-5, -5}, {10, 10}, {1, 1}};
        test.num_blits = 1;
        test.upload();

        proj_su = shaders.get_uniform("projection_mat");
        view_su = shaders.get_uniform("view_mat");
    }

    void mandelbrot_layer::tick(const input_frame &inp) {
        vec2 sc{1, 1};
        if (inp.buttons[(int) button_id::zoom_in])
            sc *= 1.025;
        if (inp.buttons[(int) button_id::zoom_out])
            sc *= 0.975;

        view_mat *= gl_mat4::transform({inp.direction * -0.125, 0}, sc, 0);
    }

    void mandelbrot_layer::upload() {
        test.upload();
    }

    void mandelbrot_layer::draw() {
        shaders.use();
        glUniformMatrix4fv(proj_su, 1, GL_FALSE, (const GLfloat *) &proj_mat);
        glUniformMatrix4fv(view_su, 1, GL_FALSE, (const GLfloat *) &view_mat);
        test.dispatch();

        ImGui::Begin((fmt::format("mandelbrot_layer@{:#016x}", reinterpret_cast<uint64_t>(this))).c_str());
        ImGui::Text("View Matrix: %s", to_string(view_mat).c_str());
        ImGui::Text("Proj Matrix: %s", to_string(proj_mat).c_str());
        ImGui::SliderFloat("Depth", &test.instances[0].center.z, -10, 10);
        ImGui::End();
    }

    void mandelbrot_layer::enqueue_render() {
        
    }
}