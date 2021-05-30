//
// Created by grant on 5/29/21.
//

#include "gl.h"

void fill_vao_and_vbo(GLuint global_vbo, render_ctx_t *ctx, unsigned max_blits) {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glGenBuffers(1, &ctx->instance_vbo);

    glGenVertexArrays(1, &ctx->vao);
    glBindVertexArray(ctx->vao);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);
    glEnableVertexAttribArray(5);

    glVertexAttribDivisor(1, 1);
    glVertexAttribDivisor(2, 1);
    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);

    glBindBuffer(GL_ARRAY_BUFFER, global_vbo);
    glVertexAttribPointer(0, 2, GL_FLOAT_T, GL_FALSE, 2 * sizeof(float), 0); // vertex vec2

    ctx->max_blits = max_blits;
    ctx->vbo_size = (GLsizeiptr) sizeof(draw_instance_t) * max_blits;
    glBindBuffer(GL_ARRAY_BUFFER, ctx->instance_vbo);
    glBufferData(GL_ARRAY_BUFFER, ctx->vbo_size, NULL, GL_DYNAMIC_DRAW);

    // 3 vec4s in the mat3 (2d model matrix)
    glVertexAttribPointer(1, 4, GL_FLOAT_T, GL_FALSE, sizeof(draw_instance_t), 0);
    glVertexAttribPointer(2, 4, GL_FLOAT_T, GL_FALSE, sizeof(draw_instance_t), (const void *) (sizeof(float) * 4));
    glVertexAttribPointer(3, 4, GL_FLOAT_T, GL_FALSE, sizeof(draw_instance_t), (const void *) (sizeof(float) * 8));

    // vec 2 for bottom left corner of sampling rect
    glVertexAttribPointer(4, 2, GL_FLOAT_T, GL_FALSE, sizeof(draw_instance_t), (const void *) (sizeof(float) * 12));

    // vec 2 for extent of sampling rect (width, height)
    glVertexAttribPointer(5, 2, GL_FLOAT_T, GL_FALSE, sizeof(draw_instance_t), (const void *) (sizeof(float) * 14));


    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void render(render_ctx_t *ctx) {
    glBindFramebuffer(GL_FRAMEBUFFER, ctx->framebuffer);
    glDrawArraysInstanced(GL_QUADS, 0, 4, (GLsizei) ctx->num_blits);
}