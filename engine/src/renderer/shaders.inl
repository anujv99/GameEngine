
static constexpr pvchar DX_V_RENDERER_SHADER[] = R"(
		cbuffer MVP : register(b0) {
			matrix <float, 4, 4> projectionView;
		};

		struct VSOut {
			float4 oPos : SV_POSITION;
			float2 oTexCoord : OUT_COORD;
			int oTexIndex : OUT_TEX_INDEX;
			float4 oColor : OUT_COLOR;
		};

		VSOut main(float2 iPos : POSITION, float2 iTexCoord : TEX_COORD, int iTexIndex : TEX_INDEX, float4 iColor : COLOR) {
			VSOut vso;
			vso.oPos = mul(projectionView, float4(iPos.x, iPos.y, 0.0f, 1.0f));
			vso.oTexCoord = iTexCoord;
			vso.oTexIndex = iTexIndex;
			vso.oColor = iColor;
			return vso;
		}
)";

static constexpr pvchar DX_F_RENDERER_SHADER[] = R"(
		
		Texture2D tex0 : register(t0);
		SamplerState splr0 : register(s0);

		Texture2D tex1 : register(t1);
		SamplerState splr1 : register(s1);

		Texture2D tex2 : register(t2);
		SamplerState splr2 : register(s2);

		Texture2D tex3 : register(t3);
		SamplerState splr3 : register(s3);

		Texture2D tex4 : register(t4);
		SamplerState splr4 : register(s4);

		Texture2D tex5 : register(t5);
		SamplerState splr5 : register(s5);

		struct VSIn {
			float4 iPos : SV_POSITION;
			float2 iTexCoord : OUT_COORD;
			int iTexIndex : OUT_TEX_INDEX;
			float4 iColor : OUT_COLOR;
		};

		float4 main(VSIn vsi) : SV_TARGET {
			float4 outCol;

			if (vsi.iTexIndex < 0)		 outCol = float4(1.0f, 1.0f, 1.0f, 1.0f);
			else if (vsi.iTexIndex == 0) outCol = tex0.Sample(splr0, vsi.iTexCoord);
			else if (vsi.iTexIndex == 1) outCol = tex1.Sample(splr1, vsi.iTexCoord);
			else if (vsi.iTexIndex == 2) outCol = tex2.Sample(splr2, vsi.iTexCoord);
			else if (vsi.iTexIndex == 3) outCol = tex3.Sample(splr3, vsi.iTexCoord);
			else if (vsi.iTexIndex == 4) outCol = tex4.Sample(splr4, vsi.iTexCoord);
			else if (vsi.iTexIndex == 5) outCol = tex5.Sample(splr5, vsi.iTexCoord);

			return outCol * vsi.iColor;
		}
)";

static constexpr pvchar GL_V_RENDERER_SHADER[] = R"(
		#version 450 core

		layout (std140, binding = 0) uniform MVP {
			mat4 projectionView;
		};

		layout (location = 0) in vec2 iPos;
		layout (location = 1) in vec2 iTexCoord;
		layout (location = 2) in int iTexIndex;
		layout (location = 3) in vec4 iColor;

		out vec2 passTexCoord;
		out vec4 passColor;
		flat out int passTexIndex;

		void main() {
			passTexCoord = vec2(iTexCoord.x, 1.0f - iTexCoord.y);
			passTexIndex = iTexIndex;
			passColor = iColor;
			gl_Position = projectionView * vec4(iPos.x, iPos.y, 0.0f, 1.0f);
		}
)";

static constexpr pvchar GL_F_RENDERER_SHADER[] = R"(
		#version 450 core

		layout (binding = 0) uniform sampler2D tex0;
		layout (binding = 1) uniform sampler2D tex1;
		layout (binding = 2) uniform sampler2D tex2;
		layout (binding = 3) uniform sampler2D tex3;
		layout (binding = 4) uniform sampler2D tex4;
		layout (binding = 5) uniform sampler2D tex5;

		out vec4 FragColor;

		in vec2 passTexCoord;
		in vec4 passColor;
		flat in int passTexIndex;

		void main() {
			vec4 outCol;

			if (passTexIndex < 0)	    outCol = vec4(1.0f);					
			else if (passTexIndex == 0) outCol = texture(tex0, passTexCoord);
			else if (passTexIndex == 1) outCol = texture(tex1, passTexCoord);
			else if (passTexIndex == 2) outCol = texture(tex2, passTexCoord);
			else if (passTexIndex == 3) outCol = texture(tex3, passTexCoord);
			else if (passTexIndex == 4) outCol = texture(tex4, passTexCoord);
			else if (passTexIndex == 5) outCol = texture(tex5, passTexCoord);

			FragColor = outCol * passColor;
		}
)";