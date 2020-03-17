static constexpr pvchar DX_V_FRAMEBUFFER_SHADER[] = R"(
	struct VSOut {
		float4 oPos : SV_POSITION;
		float2 oTexCoord : OUT_COORD;
	};

	VSOut main(float2 iPos : POSITION, float2 iTexCoord : TEX_COORD) {
		VSOut vso;
		vso.oPos = float4(iPos.x, iPos.y, 0.0f, 1.0f);
		vso.oTexCoord = iTexCoord;
		return vso;
	}
)";

static constexpr pvchar DX_F_FRAMEBUFFER_SHADER[] = R"(
	Texture2D tex : register(t0);
	SamplerState splr : register(s0);

	struct VSIn {
		float4 iPos : SV_POSITION;
		float2 iTexCoord : OUT_COORD;
	};

	float4 main(VSIn vsi) : SV_TARGET {
		return tex.Sample(splr, vsi.iTexCoord);
	}
)";

static constexpr pvchar GL_V_FRAMEBUFFER_SHADER[] = R"(
	#version 450 core

	layout (location = 0) in vec2 iPos;
	layout (location = 1) in vec2 iTexCoord;

	out vec2 passTexCoord;

	void main() {
		passTexCoord = vec2(iTexCoord.x, 1.0f - iTexCoord.y);
		gl_Position = vec4(iPos.xy, 0.0f, 1.0f);
	}
)";

static constexpr pvchar GL_F_FRAMEBUFFER_SHADER[] = R"(
	#version 450 core

	layout (binding = 0) uniform sampler2D tex;

	out vec4 FragColor;

	in vec2 passTexCoord;

	void main() {
		FragColor = texture(tex, passTexCoord);
	}
)";