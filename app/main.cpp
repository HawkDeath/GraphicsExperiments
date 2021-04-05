#include <BaseApplication.h>
#include <Log/Log.h>
#include <Renderer/Shader.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <memory>
namespace shader {
const char *vs = "#version 330 core\n"
                 "layout(location = 0) in vec3 aPos;\n"
                 "layout(location = 1) in vec3 aColor;\n"
                 "layout(location = 2) in vec2 aTexCoord;\n"
                 "out vec3 ourColor;\n"
                 "out vec2 TexCoord;\n"
                 "void main() {\n"
                 "  gl_Position = vec4(aPos, 1.0);\n"
                 "  ourColor = aColor;\n"
                 "  TexCoord = vec2(aTexCoord.x, aTexCoord.y);\n"
                 "}";

const char *fs = "#version 330 core\n"
                 "out vec4 FragColor;\n"
                 "in vec3 ourColor;\n"
                 "in vec2 TexCoord;\n"
                 "uniform sampler2D texture1;\n"
                 "uniform sampler2D texture2;\n"
                 "void main() {\n"
                 "  FragColor =    mix(texture(texture1, TexCoord), "
                 "texture(texture2, TexCoord), 0.8);\n"
                 "}";

} // namespace shader

class Application : public Leprechaun::BaseApplication {
public:
  Application(int argc, char **argv) {
    Leprechaun::ApplicationConfig defaultConfig;
    Leprechaun::BaseApplication(argc, argv, defaultConfig);
  }
  ~Application() {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
  }

protected:
  void onInitialize() override {
    Leprechaun::Api::ShaderDescription shaderDesc;
    shaderDesc.vertexShaderSources = shader::vs;
    shaderDesc.fragmenShaderSources = shader::fs;
    shaderDesc.name = "texture_shader";
    shader = std::make_unique<Leprechaun::Api::Shader>(shaderDesc);

    float vertices[] = {
        // positions          // colors           // texture coords
        0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
        0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
        -0.5f, 0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f  // top left
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
                 GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                          (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                          (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                    GL_REPEAT); // set texture wrapping to GL_REPEAT (default
                                // wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(
        true); // tell stb_image.h to flip loaded texture's on the y-axis.
    // The FileSystem::getPath(...) is part of the GitHub repository so we can
    // find files on any IDE/platform; replace it with your own image path.
    unsigned char *data = stbi_load("C:\\Users\\patry\\Pictures\\4k-images_110628423_312.jpg", &width,
        &height, &nrChannels, 0);
    if (data) {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                   GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);
    } else {
      ELOG("Failed to load texture")
    }
    stbi_image_free(data);
    // texture 2
    // ---------
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                    GL_REPEAT); // set texture wrapping to GL_REPEAT (default
                                // wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    data = stbi_load("C:\\Users\\patry\\Pictures\\138450035_3628861020527913_5281659036981762286_n.png",
                         //        "resources/textures/awesomeface.png",
        &width, &height, &nrChannels, 0);
    if (data) {
      // note that the awesomeface.png has transparency and thus an alpha
      // channel, so make sure to tell OpenGL the data type is of GL_RGBA
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                   GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);
    } else {
      ELOG("Failed to load texture")
    }
    stbi_image_free(data);
    shader->use();

    shader->setInt("texture1", 0);
    shader->setInt("texture2", 1);

  }

  void onUpdate(const float &delata) { (void)delata; }

  void onDraw() override {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    shader->use();

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    shader->unUse();
  }

private:
  uint32_t texture1;
  uint32_t texture2;
  GLuint vbo, vao, ebo;
  std::unique_ptr<Leprechaun::Api::Shader> shader;
};

IMPLEMENT_DEMO(Application)
