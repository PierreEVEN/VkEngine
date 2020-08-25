#version 450

layout(binding = 0) uniform UniformBufferObject {
    mat4 worldProjection;
    mat4 viewMatrix;
	vec3 cameraLocation;
	float time;
} ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in vec3 inNormal;
layout(location = 4) in vec3 inTangent;

layout(push_constant) uniform PushConsts {
	mat4 model;
} primitive;

layout (location = 0) out vec3 outWorldPos;
layout (location = 1) out vec3 outNormal;
layout (location = 2) out vec3 camPos;
layout (location = 3) out vec2 texCoords;
layout (location = 4) out float outTime;

out gl_PerVertex {
    vec4 gl_Position;
};

void main() {
	vec3 locPos = vec3(primitive.model * vec4(inPosition, 1.0));
	outWorldPos = locPos;
	outNormal = mat3(primitive.model) * inNormal;
	texCoords = inTexCoord;
	camPos = ubo.cameraLocation;
	outTime = ubo.time;
	gl_Position = ubo.worldProjection * ubo.viewMatrix * primitive.model * vec4(inPosition.xyz, 1.0);

} 