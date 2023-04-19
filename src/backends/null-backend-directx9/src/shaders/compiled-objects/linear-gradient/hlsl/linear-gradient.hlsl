struct ps_input_t {
    float4 position : POSITION0;
    float2 uv : TEXCOORD0;
    float4 color : COLOR0;
};

//@note: ����� ������� ��������� �������� � ��������� � � ���� ���� ����, ������� � ��� �� ����� ������ ������ ������� �� �������� hlsl
//       https://geidav.wordpress.com/2013/03/05/hidden-hlsl-performance-hit-accessing-unpadded-arrays-in-constant-buffers/
float angle : register(c0);
float stops_count : register(c1); //@note: � ���������, i �������� ������� ����� � ������� ������������ �����������((9((
float4 stops[16] : register(c2); //@note: ����� ������� ��������� � [4], �� ������� � �������� � ��������� ��� ������ �� �������
float4 colors[16] : register(c18);

float4 main(ps_input_t input) : COLOR {
    float2 uv = input.uv - 0.5f;
    float t = 0.5f + length(uv) * cos(atan2(-uv.y, uv.x) + angle);
    
    float4 color = colors[0] / 255.f;
    for (int i = 1; i < stops_count; ++i) {
        color = lerp(color, colors[i] / 255.f, smoothstep(stops[i - 1].x, stops[i].x, t));
    }
    return color;
}