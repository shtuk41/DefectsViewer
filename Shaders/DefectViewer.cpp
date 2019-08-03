#include "DefectViewer.h"
#include "utility.h"
#include "Color.h"
#include "reportreader.h"

Scene *Scene::scene = 0;
DefectViewer * DefectViewer::defectViewer = 0;

Color::Names ConvertDefectNameToColor(std::string defect)
{
	if (defect.compare("Scratch") == 0)
	{
		return Color::RED;
	}
	else if (defect.compare("OrangePeel") == 0)
	{
		return Color::ORANGE;
	}
	else if (defect.compare("StraightGapEdgeDetection") == 0)
	{
		return Color::YELLOW;
	}
	else if (defect.compare("CornerGapEdgeDetection") == 0)
	{
		return Color::GREEN;
	}
	else if (defect.compare("CornerGapDetection") == 0)
	{
		return Color::LIGHT_BLUE;
	}
	else if (defect.compare("Waviness") == 0)
	{
		return Color::BLUE;
	}
	else if (defect.compare("BossReadThru") == 0)
	{
		return Color::MAGENTA;
	}
	else
	{
		return Color::BLACK;
	}
}

void Scene::Init()
{
	tempMultiplier = 1.0f;
	eCamera = STATIC;
	cCameraStatic.Reset();
	cCameraStatic.SetFocus(CameraStatic::CENTER);
	cCameraStatic.Init(vec4(0,0,-200.0,1.0),vec4(0.0,0.0,0.0,1.0),vec4(0.0,1.0,0.0,0.0),60.0,1920/1080,1,300.0);
	cCameraChase.Init(vec4(20,10,0,1.0),vec4(0,10,0.0,1.0),vec4(0.0,1.0,0.0,0.0),60.0,fWindowWidth/fWindowHeight,1.0,105.0);
	cCameraViewpoint.Init(vec4(1,6,0,1.0),vec4(-10.0,6.0,0.0,1.0),vec4(0.0,10.0,0.0,0.0),60.0,fWindowWidth/fWindowHeight,1.0,105.0);

	pCameraCurrent = &cCameraStatic;

	glfRotAngleX = 0;
	glfRotAngleY = 0;

	mv = pCameraCurrent->GetMv();
	p  = pCameraCurrent->GetP();

}

void	Scene::Draw(GraphObject *object, bool update = true)
{
	mat4 mvinv = mv.inverse();

	vec4 initX = mvinv * vec4(1.0,0.0,0.0,0.0);
	vec4 initY = mvinv * vec4(0.0,1.0,0.0,0.0);

	vec3 v3initX = vec3(initX.x, initX.y, initX.z);
	vec3 v3initY = vec3(initY.x, initY.y, initY.z);

	mv = mv * Rotate(v3initX, glfRotAngleX) * Rotate(v3initY, glfRotAngleY);
	p  = pCameraCurrent->GetP();

	if (object)
	{
		if (update)
		{
			object->SetMv(mv);
		}

		object->SetP(p);
		object->Draw();
	}
}

void Scene::UpdateYRot(float angle, bool update)
{
	if (update)
	{
		glfRotAngleY = 360.0f *  ((angle - oldYRot) / (1920.0f * tempMultiplier));
	}
	else
	{
		glfRotAngleY = 0;
	}

	oldYRot = angle;
}

void Scene::UpdateXRot(float angle, bool update)
{
	if (update)
	{
		glfRotAngleX = 360.0f *  ((angle - oldXRot) / (1080.0f * tempMultiplier));
	}
	else
	{
		glfRotAngleX = 0;
	}

	oldXRot = angle;
}

void	DefectViewer::Init()
{
	DefectViewer::SceneInstance()->Init();
	background = new Background();
	background->Init(Color::GRAY);
	mat4 currentmv = SceneInstance()->GetMv() * Translate(45,-100,0);
	background->SetMv(currentmv);
	widget = new Widget();
	widget->Init(Color::LIGHT_GRAY);
}

void	DefectViewer::DrawObjects()
{
	SceneInstance()->SetMultiplier(pointers.size() > 0 ? (float)pointers.size() : 1.0f);
	SceneInstance()->Draw(background, false);
	SceneInstance()->Draw(widget);

	for (vector<Pointer*>::iterator it = pointers.begin(); it != pointers.end(); ++it)
	{
		SceneInstance()->Draw(*it);
	}
}

void	DefectViewer::RepositionBottomPlane(Pointer *p, float width, float height)
{
	width = Utility::MinMax(width,0,6);
	height = Utility::MinMax(height,0,71);
	p->InitTransform(height,-width, 0,-90,0,0);
}

void	DefectViewer::RepositionRightPlane(Pointer *p, float width, float height)
{
	width = Utility::MinMax(width,0,6);
	height = Utility::MinMax(height,0,144);
	p->InitTransform(-width,height,0,0,90,0);
}

void	DefectViewer::RepositionLeftPlane(Pointer *p, float width, float height)
{
	width = Utility::MinMax(width,0,6);
	height = Utility::MinMax(height,0,144);
	p->InitTransform(width,height,-71,0,-90,0);
}

void	DefectViewer::RepositionTopPlane(Pointer *p, float width, float height)
{
	width = Utility::MinMax(width,0,6);
	height = Utility::MinMax(height,0,71);
	p->InitTransform(height,width,-144,90,0,0);
}

void	DefectViewer::ReloadPointers()
{
	if (pointers.size() > 0)
	{
		for (std::vector<Pointer*>::iterator it = pointers.begin(); it != pointers.end(); ++it)
		{
			delete *it;
		}

		pointers.clear();
	}
	
	std::vector<report_reader::Defect> &d = report_reader::ReportReader::GetData();

	for (std::vector<report_reader::Defect>::iterator it  = d.begin(); it != d.end(); ++it)
	{
		report_reader::Defect def = *it;
		std::string fov		= def.GetFov();
		std::string kind	= def.GetKind();
		float x = (float)def.GetX();
		float y = (float)def.GetY();

		if (fov.compare("Bottom_Edge_Short") == 0)
		{
			Pointer *p = new Pointer();

			p->Init(ConvertDefectNameToColor(kind));

			x = Utility::MinMax(x, 338.0f, 833.0f);
			y = Utility::MinMax(y, 771.0f, 3183.0f - 1.0f);

			float width;

			if (y >= 771.0 && y < 881.0)
			{
				width = 0;
			}
			else if (y >= 881.0 && y < 1836.0)
			{
				width = 0.0f + (y - 881.0f) / (1836.0f - 881.0f) * 36.0f;
			}
			else if (y >= 1836.0f && y < 3183.0)
			{
				width = 36.0f + (y - 1836.0f) / (3183.0f - 1836.0f) * 36.0f;
			}

			float height = 6.0f / (833.0f - 338.0f) * (x - 338.0f);

			RepositionBottomPlane(p, height, width);
			pointers.push_back(p);
		}
		else if (fov.compare("Bottom_Radius_Short") == 0)
		{
			Pointer *p = new Pointer();

			p->Init(ConvertDefectNameToColor(kind));

			float width;

			x = Utility::MinMax(x, 320.0f, 800.0f);
			y = Utility::MinMax(y, 288.0,  2978.0f - 1.0f);
			
			if (y >= 276.0f && y < 424.0f)
			{
				width = 0;
			}
			else if (y >= 424.0f && y < 1384.0f)
			{
				width = 0.0f + (y - 424.0f) / (1384.0f - 424.0f) * 36.0f;
			}
			else if (y >= 1384.0f && y < 2978.0)
			{
				width = 36.0f + (y - 1384.0f) / (2978.0f - 1384.0f) * 36.0f;
			}

			float height = 6.0f / (808.0f - 320.0f) * (x - 320.0f);

			RepositionBottomPlane(p, height, width);
			pointers.push_back(p);
		}
		else if (fov.compare("Bottom_RTEdge_Short") == 0)
		{
			x = Utility::MinMax(x, 453.0,  942.0);
			y = Utility::MinMax(y, 735.0,  2978.0f -1.0f);

			Pointer *p = new Pointer();

			p->Init(ConvertDefectNameToColor(kind));

			float width;

			if (y >= 735 && y < 885)
			{
				width = 0;
			}
			else if (y >= 885.0 && y < 1836.0)
			{
				width = 0.0f + (y - 885.0f) / (1836.0f - 885.0f) * 36.0f;
			}
			else if (y >= 1836.0 && y < 2978.0)
			{
				width = 36.0f + (y - 1836.0f) / (2978.0f - 1836.0f) * 36.0f;
			}

			float height = 6.0f / (942.0f - 453.0f) * (x - 453.0f);

			RepositionBottomPlane(p, 6.0f-height, 72.0f - width);
			pointers.push_back(p);
		}
		else if (fov.compare("Bottom_RTRadius_Short") == 0)
		{
			x = Utility::MinMax(x, 467.0f, 781.0f);
			y = Utility::MinMax(y, 319.0,  2656.0f-1.0f);

			Pointer *p = new Pointer();

			p->Init(ConvertDefectNameToColor(kind));

			float width;

			if (y >= 319.0f && y < 1485.0f)
			{
				width = 0.0f + (y - 319.0f) / (1485.0f - 319.0f) * 36.0f;
			}
			else if (y >=  1485.0 && y < 2656.0f)
			{
				width = 36.0f + (y - 1485.0f ) / (2656.0f - 1485.0f ) * 36.0f;
			}

			float height = 3.0f / (781.0f - 467.0f) * (x - 467.0f);

			RepositionBottomPlane(p, 6.0f-height, 72.0f - width);
			pointers.push_back(p);
		}
		else if (fov.compare("Left_Edge_Long") == 0)
		{
			x = Utility::MinMax(x, 290.0f, 632.0f);
			y = Utility::MinMax(y, 44.0f, 5219.0f);

			Pointer *p = new Pointer();

			p->Init(ConvertDefectNameToColor(kind));

			float height = 144.0f / (5219.0f - 44.0f) * (y - 44.0f);
			float width	 = 6.0f / (632.0f - 290.0f) * (x - 290.0f);

			RepositionRightPlane(p, width, 144.0f - height);

			pointers.push_back(p);
		}
		else if (fov.compare("Left_Radius_Long") == 0)
		{
			x = Utility::MinMax(x, 258.0f, 486.0f);
			y = Utility::MinMax(y, 396.0f, 5496.0f);

			Pointer *p = new Pointer();

			p->Init(ConvertDefectNameToColor(kind));

			float height = 144.0f / (5496.0f - 396.0f) * (y - 396.0f);
			float width	 = 6.0f / (486.0f - 258.0f) * (x - 258.0f);

			RepositionRightPlane(p, 6.0f - width, 144.0f - height);

			pointers.push_back(p);
		}
		else if (fov.compare("Left_RTEdge_Long") == 0)
		{
			x = Utility::MinMax(x, 231.0f, 539.0);
			y = Utility::MinMax(y, 88.0f, 5148.0f);

			Pointer *p = new Pointer();

			p->Init(ConvertDefectNameToColor(kind));

			float height = 144.0f / (5148.0f - 88.0f) * (y - 88.0f);
			float width	 = 6.0f / (539.0f - 231.0f) * (x - 231.0f);

			RepositionRightPlane(p, 6.0f - width, height);

			pointers.push_back(p);
		}
		else if (fov.compare("Left_RTRadius_Long") == 0)
		{
			x = Utility::MinMax(x, 312.0f, 588.0f);
			y = Utility::MinMax(y, 380.0f, 5536.0f);

			Pointer *p = new Pointer();

			p->Init(ConvertDefectNameToColor(kind));

			float height = 144.0f / (5536.0f - 380.0f) * (y - 380.0f);
			float width	 = 6.0f / (588.0f - 312.0f) * (x - 312.0f);

			RepositionRightPlane(p, width, height);

			pointers.push_back(p);
		}
		else if (fov.compare("Top_Edge_Short") == 0)
		{
			x = Utility::MinMax(x, 318.0f, 818.0f);
			y = Utility::MinMax(y, 758.0f, 2948.0f);

			Pointer *p = new Pointer();

			p->Init(ConvertDefectNameToColor(kind));

			float height = 72.0f / (2948.0f - 758.0f) * (y - 758.0f);
			float width	 = 6.0f / (818.0f - 318.0f) * (x - 318.0f);

			RepositionTopPlane(p, width, 72.0f - height);

			pointers.push_back(p);
		}
		else if (fov.compare("Top_Radius_Short") == 0)
		{
			x = Utility::MinMax(x, 330.0f, 781.0f);
			y = Utility::MinMax(y, 308.0f, 2678.0f);

			Pointer *p = new Pointer();

			p->Init(ConvertDefectNameToColor(kind));

			float height = 72.0f / (2678.0f - 308.0f) * (y - 308.0f);
			float width	 = 6.0f / (781.0f - 330.0f) * (x - 330.0f);

			RepositionTopPlane(p, width, 72.0f - height);

			pointers.push_back(p);
		}
		else if (fov.compare("Top_RTEdge_Short") == 0)
		{
			x = Utility::MinMax(x, 504.0f, 1000.0f);
			y = Utility::MinMax(y, 765.0f, 3039.0f);

			Pointer *p = new Pointer();

			p->Init(ConvertDefectNameToColor(kind));

			float height = 72.0f / (3039.0f - 765.0f) * (y -  765.0f);
			float width	 = 6.0f / (1000.0f- 504.0f) * (x - 504.0f);

			RepositionTopPlane(p, 6.0f - width, height);

			pointers.push_back(p);
		}
		else if (fov.compare("Top_RTRadius_Short") == 0)
		{
			x = Utility::MinMax(x, 423.0f, 896.0f);
			y = Utility::MinMax(y, 374.0f, 2761.0f);

			Pointer *p = new Pointer();

			p->Init(ConvertDefectNameToColor(kind));

			float height = 72.0f / (2761.0f - 374.0f) * (y -  374.0f);
			float width	 = 6.0f / (896.0f- 423.0f) * (x - 423.0f);

			RepositionTopPlane(p, 6.0f - width, height);

			pointers.push_back(p);
		}
		else if (fov.compare("Right_Edge_Long") == 0)
		{
			x = Utility::MinMax(x, 313.0f, 638.0f);
			y = Utility::MinMax(y, 77.0f, 5219.0f);

			Pointer *p = new Pointer();

			p->Init(ConvertDefectNameToColor(kind));

			float height = 144.0f / (5219.0f - 77.0f) * (y - 77.0f);
			float width	 = 6.0f / (638.0f - 313.0f) * (x - 313.0f);

			RepositionLeftPlane(p, width, height);

			pointers.push_back(p);
		}
		else if (fov.compare("Right_Radius_Long") == 0)
		{
			x = Utility::MinMax(x, 258.0f, 474.0f);
			y = Utility::MinMax(y, 378.0f, 5560.0f);

			Pointer *p = new Pointer();

			p->Init(ConvertDefectNameToColor(kind));

			float height = 144.0f / (5560.0f - 378.0f) * (y - 378.0f);
			float width	 = 6.0f / (474.0f - 258.0f) * (x - 258.0f);

			RepositionLeftPlane(p, 6.0f-width, height);

			pointers.push_back(p);
		}
		else if (fov.compare("Right_RTEdge_Long") == 0)
		{
			x = Utility::MinMax(x, 252.0f, 548.0f);
			y = Utility::MinMax(y, 72.0f, 5228.0f);

			Pointer *p = new Pointer();

			p->Init(ConvertDefectNameToColor(kind));

			float height = 144.0f / (5228.0f - 72.0f) * (y - 72.0f);
			float width	 = 6.0f / (548.0f - 252.0f) * (x - 252.0f);

			RepositionLeftPlane(p, 6.0f-width, 144.0f - height);

			pointers.push_back(p);
		}
		else if (fov.compare("Right_RTRadius_Long") == 0)
		{
			x = Utility::MinMax(x, 328.0f, 548.0f);
			y = Utility::MinMax(y, 372.0f, 5228.0f);

			Pointer *p = new Pointer();

			p->Init(ConvertDefectNameToColor(kind));

			float height = 144.0f / (5228.0f - 372.0f) * (y - 372.0f);
			float width	 = 6.0f / (548.0f - 328.0f) * (x - 328.0f);

			RepositionLeftPlane(p, width, 144.0f - height);

			pointers.push_back(p);
		}
	}
}

DefectViewer::~DefectViewer() 
{ 
	if (background != 0)
	{
		delete background;
		background = 0;
	}

	if (widget != 0)
	{
		delete widget;
		widget = 0;
	}

	for (vector<Pointer*>::iterator it = pointers.begin(); it != pointers.end(); ++it)
	{
		delete *it;
	}


	if (defectViewer) 
		delete defectViewer;  

	defectViewer = 0;
}
