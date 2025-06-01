#version 330 core

in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

uniform vec3 lightPos;
uniform vec3 viewPos;

uniform vec3 ka;
uniform vec3 kd;
uniform vec3 ks;
uniform float shininess;
uniform float ambientIntensity;
uniform vec3 lightColor;

void main()
{
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    // ambient
    vec3 ambient = ambientIntensity * ka;

    // diffuse
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = kd * diff;

    // specular
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = ks * spec;

    vec3 color = lightColor * (ambient + diffuse + specular);

    // Gamma correction
    vec3 gammaCorrected = pow(color, vec3(1.0 / 2.2));
    FragColor = vec4(gammaCorrected, 1.0);
}
