#pragma once
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Camera.h"



using namespace engine;
Renderer::Renderer():
d3d(NULL),
d3d_dev(NULL),
p_vb(NULL),
p_ib(NULL),
m_frustum(new Frustum(this))
//p_vTexture(NULL)
{
	// Again, Nothing to do.
}

Renderer::~Renderer(){

	if (c){
		delete c;
		c = NULL;
	}

	/*
	if (p_vTexture){
		delete p_vTexture;
		p_vTexture = NULL;
	}
	*/

	if(d3d_dev){
		d3d_dev->Release();
		d3d_dev = NULL;
	}

	if(d3d){
		d3d->Release();
		d3d = NULL;
	}
	
	if (m_frustum){
		delete m_frustum;
		m_frustum = NULL;
	}

	for (std::vector<Texture>::iterator it = m_vTexture.begin(); it != m_vTexture.end(); it++){
		(*it)->Release();
		(*it) = NULL;
	}
	
	m_vTexture.clear();
}

bool Renderer::Init(HWND _HwnD){
	d3d = Direct3DCreate9(D3D_SDK_VERSION);
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = true;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = _HwnD;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;

	if(d3d->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, _HwnD, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &d3d_dev) == D3D_OK){

		d3d_dev->SetRenderState(D3DRS_LIGHTING, FALSE); // Cambiar a TRUE para habilitar la luz.
		d3d_dev->SetRenderState(D3DRS_ZENABLE, TRUE);

		d3d_dev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		d3d_dev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		d3d_dev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		d3d_dev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		d3d_dev->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		d3d_dev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

		
		D3DVIEWPORT9 kViewport; //CAMARA 
		d3d_dev->GetViewport(&kViewport);

		float fViewPortWidth = static_cast<float>(kViewport.Width);
		float fViewPortHeight = static_cast<float>(kViewport.Height);

		D3DXMATRIX projectionMatrix;
		D3DXMatrixPerspectiveFovLH(&projectionMatrix, D3DXToRadian(90), fViewPortWidth / fViewPortHeight, 1.0f, 1000.0f);
		//D3DXMatrixOrthoLH(&projectionMatrix,fViewPortWidth,fViewPortHeight, -1.0f, 1.0f);
		d3d_dev->SetTransform(D3DTS_PROJECTION, &projectionMatrix);

		/*
		// view
		D3DXMATRIX viewMatrix;
		D3DXVECTOR3 Position(0.0f, 0.0f, -500.0f);
		D3DXVECTOR3 Target(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 Up(0.0f, 1.0f, 0.0f);
		D3DXMatrixLookAtLH(&viewMatrix, &Position, &Target, &Up);
		d3d_dev->SetTransform(D3DTS_VIEW, &viewMatrix);
		*/
		// world
		D3DXMATRIX worldMatrix;
		D3DXMatrixIdentity(&worldMatrix);
		d3d_dev->SetTransform(D3DTS_WORLD, &worldMatrix);

		// Creo mi camara...
		c = new Camera();
		c->Init(this);

		return true;
	}
	return false;
}

void Renderer::SwitchLightning(){
	DWORD value;
	d3d_dev->GetRenderState(D3DRS_LIGHTING,&value);
	if ((bool)value)
	{
		d3d_dev->SetRenderState(D3DRS_LIGHTING, false);
	}
	else
	{
		d3d_dev->SetRenderState(D3DRS_LIGHTING, true);
	}
	
}

void Renderer::BeginFrame(){
	d3d_dev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(r, g, b), 1.0f, 0);
	d3d_dev->BeginScene();
}

void Renderer::SetBackgroundColor(short _r,short _g, short _b){
	r = _r;
	g = _g;
	b = _b;
}

void Renderer::EndFrame(){
	d3d_dev->EndScene();
	d3d_dev->Present(NULL, NULL, NULL, NULL);
}

// TODO: Erase Primitive Type ???
D3DPRIMITIVETYPE primitiveMap[engine::PrimitiveCount] = {
	D3DPT_TRIANGLELIST,
	D3DPT_TRIANGLESTRIP,
	D3DPT_POINTLIST,
	D3DPT_LINELIST,
	D3DPT_LINESTRIP,
	D3DPT_TRIANGLEFAN
};


D3DTRANSFORMSTATETYPE MatrixTypeMapping[MatrixTypeCount] ={
	 D3DTS_VIEW,
	 D3DTS_PROJECTION,
	 D3DTS_WORLD
};

void Renderer::SetMatrix(MatrixType matrixType, const Matrix& matrix){
	d3d_dev->SetTransform(MatrixTypeMapping[matrixType], matrix);
}

void Renderer::setTransformMatrix(D3DXMATRIX* kMatrix) {
	// set the matrix
	d3d_dev->MultiplyTransform(D3DTS_VIEW, kMatrix);
}


void Renderer::loadIdentity() {
	D3DXMATRIX kTempMatrix;

	// set identity matrix
	D3DXMatrixIdentity(&kTempMatrix);

	// if it is a view matrix, use default values


	D3DXVECTOR3 kEyePos(0, 0, -1);
	D3DXVECTOR3 kLookPos(0, 0, 0);
	D3DXVECTOR3 kUpVector(0, 1, 0);

	// generate the view matrix
	D3DXMatrixLookAtLH(&kTempMatrix, &kEyePos, &kLookPos, &kUpVector);

	// set the matrix
	d3d_dev->SetTransform(D3DTS_VIEW, &kTempMatrix);
}

void Renderer::SetTransformMatrix(D3DXMATRIX* kMatrix){
	// set the matrix
	d3d_dev->MultiplyTransform(D3DTS_VIEW, kMatrix);
}

void Renderer::Draw(ColorVertex* /*DIBUJA QUAD*/ v, Primitive p, size_t vC){
	//p_vb->SetVertexData(v, vC);
	//p_vb->Draw(Primitive::PointList);
}

void Renderer::Draw(TextureVertex*/*DIBUJA LA TEXTURA*/ v, Primitive p, size_t vC){
	//p_vTexture->SetVertexData(v, primitiveMap[p], vC);
	//p_vTexture->Draw();
}

void Renderer::Draw(engine::Primitive p){
	int primitive = 0;

	if (p < 0 || p > 6)
		p = Primitive::TriangleList;

	D3DPRIMITIVETYPE pType = primitiveMap[p];

	if(pType == D3DPT_POINTLIST){
		primitive = p_ib->indexCount();
	}
	else if(pType == D3DPT_LINELIST){
		primitive = p_ib->indexCount() / 2;
	}
	else if(pType == D3DPT_LINESTRIP){
		primitive = p_ib->indexCount() - 1;
	}
	else if(pType == D3DPT_TRIANGLELIST){
		primitive = p_ib->indexCount() / 3;
	}
	else if(pType == D3DPT_TRIANGLESTRIP){
		primitive = p_ib->indexCount() - 2;
	}
	else if(pType == D3DPT_TRIANGLEFAN){
		primitive = p_ib->indexCount() - 2;
	}
	d3d_dev->DrawIndexedPrimitive(primitiveMap[p], 0, 0, p_vb->vertexCount(), 0, primitive);
}

const Texture Renderer::LoadTexture(const std::string& Fname, int KeyCode){
	IDirect3DTexture9* p_Texture = NULL;
	HRESULT HR = D3DXCreateTextureFromFileEx(d3d_dev,
											Fname.c_str(),
											0, 0, 0, 0,
											D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
											D3DX_FILTER_NONE, D3DX_FILTER_NONE,
											KeyCode,
											NULL,
											NULL,
											&p_Texture);
	if (HR != D3D_OK){
		return NoTexture;
	}
	else{
		m_vTexture.push_back(p_Texture);
		return p_Texture;
	}
}


VertexBuffer* Renderer::CreateVertexBuffer(size_t vSize, unsigned int FVF){
	return new VertexBuffer(*this, d3d_dev, vSize, FVF);
}


void Renderer::SetCurrentVertexBuffer(VertexBuffer* c_vb){
	p_vb = c_vb;
}


IndexBuffer* Renderer::CreateIndexBuffer(){
	return new IndexBuffer(*this, d3d_dev);
}


void Renderer::SetCurrentIndexBuffer(IndexBuffer* c_ib){
	p_ib = c_ib;
}


void Renderer::SetCurrentTexture(const Texture& texture){
	d3d_dev->SetTexture(0, texture);
}

void Renderer::CalculateFrustrum() {
	D3DXMATRIX MatrizProy;
	D3DXMATRIX MatrizVista;

	d3d_dev->GetTransform(D3DTS_VIEW, &MatrizVista);
	d3d_dev->GetTransform(D3DTS_PROJECTION, &MatrizProy);

	this->m_frustum->Calculate(&MatrizVista, &MatrizProy);
}