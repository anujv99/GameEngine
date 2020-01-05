
static constexpr pvchar DX_V_SHADER[] = R"(
		cbuffer MVP : register(b0) {
			matrix <float, 4, 4> projectionView;
		};

		struct VSOut {
			float4 oPos : SV_POSITION;
			float4 oColor : OUT_COLOR;
		};

		VSOut main(float2 iPos : POSITION, float4 iColor : COLOR) {
			VSOut vso;
			vso.oPos = mul(projectionView, float4(iPos.x, iPos.y, 0.0f, 1.0f));
			vso.oColor = iColor;
			return vso;
		}
)";

static constexpr pvchar DX_F_SHADER[] = R"(
		struct VSIn {
			float4 iPos : SV_POSITION;
			float4 iColor : OUT_COLOR;
		};

		float4 main(VSIn vsi) : SV_TARGET {
			return vsi.iColor;
		}
)";

static constexpr pvchar GL_V_SHADER[] = R"(
		#version 450 core

		layout (std140, binding = 0) uniform MVP {
			mat4 projectionView;
		};

		layout (location = 0) in vec2 iPos;
		layout (location = 1) in vec4 iColor;

		out vec4 passColor;

		void main() {
			passColor = iColor;
			gl_Position = projectionView * vec4(iPos.x, iPos.y, 0.0f, 1.0f);
		}
)";

static constexpr pvchar GL_F_SHADER[] = R"(
		#version 450 core

		out vec4 FragColor;

		in vec4 passColor;

		void main() {
			FragColor = passColor;
		}
)";

