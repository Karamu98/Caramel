#include "TestModel.h"

void TestModel::ImGui()
{
    ImGui::LabelText(Model->GetPath().c_str(), "");
    ImGui::DragFloat3("Position", glm::value_ptr(Position), 0.1f);
    ImGui::DragFloat3("Rotation", glm::value_ptr(Rotation), 0.1f);
}

void TestModel::Render()
{
    glm::mat4 modelTransform = glm::mat4(1);

    modelTransform = glm::mat4(1);
    modelTransform = glm::translate(modelTransform, Position);
    modelTransform = glm::rotate(modelTransform, glm::radians(Rotation.x), glm::vec3(1, 0, 0));
    modelTransform = glm::rotate(modelTransform, glm::radians(Rotation.y), glm::vec3(0, 1, 0));
    modelTransform = glm::rotate(modelTransform, glm::radians(Rotation.z), glm::vec3(0, 0, 1));

    Caramel::Renderer::Submit(Model, modelTransform);
}
