#ifdef WIN32
#include <windows.h>
#endif

#include <math.h>
#include <stdio.h>

#include <GL/gl.h>

#include <GL/glu.h>

#include "eclipse.h"

#include "app/app.h"
#include "app/globals.h"
#include "app/opengl_interface.h"

#include "world/world.h"
#include "world/generator/numbergenerator.h"

#include "interface/localinterface/worldmap/worldmap_layout.h"

// ==============================================================================


WorldMapInterfaceObject::WorldMapInterfaceObject ()
{

    TYPE = WORLDMAPOBJECT_NONE;
    x = 0;
    y = 0;
    baseX = 0;
    baseY = 0;

}

WorldMapInterfaceObject::~WorldMapInterfaceObject ()
{
}

void WorldMapInterfaceObject::SetTYPE ( int newTYPE )
{

    TYPE = newTYPE;

}


void WorldMapInterfaceObject::SetPosition ( int newx, int newy )
{

    x = newx;
    y = newy;

}

void WorldMapInterfaceObject::SetBasePosition ( int newx, int newy )
{

    baseX = newx;
    baseY = newy;

}

void WorldMapInterfaceObject::Draw ( int xOffset, int yOffset, float zoom )
{

    switch ( TYPE ) {

        case WORLDMAPOBJECT_GATEWAY:
        {        
        }

        // DON'T INSERT ANY MORE HERE
        // Gateway should fall through to location

        case WORLDMAPOBJECT_LOCATION:
        {

            glColor4f ( 1.0, 1.0, 1.0, 1.0 );
            int xPos = 23 + ( (x - 23) - xOffset ) * zoom;
            int yPos = 50 + ( (y - 50) - yOffset ) * zoom;
 
            if ( xPos >= 0 && yPos >= 0 ) {
            
                glBegin ( GL_QUADS );
                    glVertex2i ( xPos, yPos );
                    glVertex2i ( xPos + 7, yPos );
                    glVertex2i ( xPos + 7, yPos + 7 );
                    glVertex2i ( xPos, yPos + 7 );
                glEnd ();

            }

            break;

        }

        default:
            break;

    };

}

MapRectangle WorldMapInterfaceObject::GetExtent () const
{
    return MapRectangle(x, y, 7, 7);
}


// ==============================================================================

WorldMapInterfaceLabel::WorldMapInterfaceLabel( 
    const MapRectangle &mapRect, 
    WorldMapInterfaceObject *newFeaturePoint, 
    const char *newCaption)

    : featurePoint(newFeaturePoint),
      caption(NULL),
      labelWidth(0)
{
    SetTYPE(WORLDMAPOBJECT_LABEL);
    SetCaption(newCaption);
    CalculatePossibleLabelPositions(mapRect);
    SetRandomLabelPosition();
}

void WorldMapInterfaceLabel::CalculatePossibleLabelPositions( const MapRectangle &mapRect )
{
    numPossLabelPos = 0;
    for (int pos = 0; pos < 8; pos++) {
	SetLabelPosition(pos);
	if (mapRect.contains(GetExtent()))
	    possLabelPos[numPossLabelPos++] = pos;
    }
}

void WorldMapInterfaceLabel::SetCaption ( const char *newcaption )
{
    if ( caption ) delete [] caption;
    caption = new char [strlen(newcaption)+1];
    strcpy ( caption, newcaption );
    CalculateWidth();
}

const char *WorldMapInterfaceLabel::GetCaption () const
{
    return caption;
}

WorldMapInterfaceLabel::~WorldMapInterfaceLabel ()
{

    if ( caption ) delete [] caption;

}

void WorldMapInterfaceLabel::Draw ( int xOffset, int yOffset, float zoom )
{
    switch (TYPE) {
    case WORLDMAPOBJECT_LABEL:
    {

        UplinkAssert (caption);
/*        
         glColor4f ( 1.0, 0.0, 0.0, 1.0 );
         glBegin ( GL_QUADS ); 
         {
         MapRectangle r = GetExtent();
         glVertex2i ( r.x1, r.y1 );
         glVertex2i ( r.x2(), r.y1 );
         glVertex2i ( r.x2(), r.y2() );
         glVertex2i ( r.x1, r.y2() );
         } 
         glEnd ();
  */      

        int dX = x - baseX;
        int dY = y - baseY;

        int xPos = 23 + ( (baseX - 23) - xOffset ) * zoom;
        int yPos = 50 + ( (baseY - 50) - yOffset ) * zoom;

        xPos += dX;
        yPos += dY;

        if ( xPos >= 0 && yPos >= 0 ) {
            glColor4f ( 1.0, 1.0, 1.0, 1.0 );        
            GciDrawText ( xPos, yPos + 7, caption );
        }

        break;

    }
    
    default:
        UplinkAssert(0);
    }
}

MapRectangle WorldMapInterfaceLabel::GetExtent () const
{    
    return MapRectangle(x, y, labelWidth, 13);
}

void WorldMapInterfaceLabel::CalculateWidth()
{
    labelWidth = GciTextWidth(caption);
//     int spaces = 0;
    
//     for (const char *s = caption; *s; s++)
// 	if (*s == ' ') 
// 	    spaces++;
    
//     labelWidth += 1 + GciTextWidth(" ")*spaces;
}

void WorldMapInterfaceLabel::SetRandomLabelPosition ()
{
    SetLabelPosition(possLabelPos[NumberGenerator::RandomNumber(numPossLabelPos)]);
}

int WorldMapInterfaceLabel::GetLabelPosition() const
{
    return labelPos;
}

void WorldMapInterfaceLabel::SetLabelPosition (int n)
{
    const MapRectangle &fp = featurePoint->GetExtent();
    const MapRectangle &cp = GetExtent();

    labelPos = n;
    
    switch (n) {
    
    case 0:
        // Top Centre
        SetPosition(fp.x1 - cp.width / 2, fp.y1 - cp.height - 1);
        break;
        
    case 1:
        // Bottom Centre
        SetPosition(fp.x1 - cp.width / 2, fp.y2() + 1);
        break;
        
        
    case 2:
        // Bottom Right
        SetPosition(fp.x2() + 1, fp.y2() + 1);
        break;
        
    case 3:
        // Right
        SetPosition(fp.x2() + 2, fp.y1);
        break;
        
    case 4: 
        // Top Right
        SetPosition(fp.x2() + 1, fp.y1 - cp.height + 1);
        break;
        
    case 5:
        // Top Left
        SetPosition(fp.x1 - cp.width, fp.y1 - cp.height + 1);
        break;
        
    case 6:
        // Left
        SetPosition(fp.x1 - cp.width - 2, fp.y1);        
        break;
        
    case 7:
        // Bottom Left
        SetPosition(fp.x1 - cp.width, fp.y2() + 1);
        break;
        
    default:
        UplinkAssert(0);
    };
}


bool WorldMapInterfaceLabel::Overlaps (WorldMapInterfaceObject *other) const
{
    return GetExtent().intersects(other->GetExtent());
}

// ==============================================================================

WorldMapObjectiveFunction::WorldMapObjectiveFunction(const MapRectangle &mapRect)
    :clipRect(mapRect),
      mapWidth(mapRect.width),
      mapHeight(mapRect.height)
{
    shadeScreen = new int [mapWidth * mapHeight];
    Reset();
}

WorldMapObjectiveFunction::~WorldMapObjectiveFunction()
{
    delete [] shadeScreen;
}

void WorldMapObjectiveFunction::Reset()
{
    memset(shadeScreen, 0, mapWidth * mapHeight * sizeof(int));
    cost = 0;
}

int WorldMapObjectiveFunction::GetCost() const
{
    return cost;
}

static inline int penalty( int numOverlaps )
{
    return numOverlaps;
}

void WorldMapObjectiveFunction::AddRect( const MapRectangle& rect )
{
    MapRectangle r = clipRect.intersection(rect);
    
    if (r.isNull())
	return;
    
    r.x1 -= clipRect.x1;
    r.y1 -= clipRect.y1;
    
    int x2 = r.x2(), y2 = r.y2();
    
    for (int y = r.y1; y <= y2; y++) {
	int *line = shadeScreen + y * mapWidth;
	for (int x = r.x1; x <= x2; x++) {
	    line[x]++;
	    if (line[x] != 1)
		cost += penalty(line[x]);
	}
    }
}

void WorldMapObjectiveFunction::SubRect( const MapRectangle& rect )
{
    MapRectangle r = clipRect.intersection(rect);
    
    if (r.isNull())
	return;
    
    r.x1 -= clipRect.x1;
    r.y1 -= clipRect.y1;
    
    int x2 = r.x2(), y2 = r.y2();
    
    for (int y = r.y1; y <= y2; y++) {
	int *line = shadeScreen + y * mapWidth;
	for (int x = r.x1; x <= x2; x++) {
	    line[x]--;
	    if (line[x] != 0)
		cost -= penalty(1 + line[x]);
	}
    }
}

void WorldMapObjectiveFunction::AddObject( const WorldMapInterfaceObject *m )
{
    AddRect(m->GetExtent());
}

void WorldMapObjectiveFunction::SubObject( const WorldMapInterfaceObject *m )
{
    SubRect(m->GetExtent());
}

// ==============================================================================

WorldMapLayout::WorldMapLayout(const MapRectangle &newMapRectangle)
    : mapRectangle(newMapRectangle),
      objective(newMapRectangle)
{
    Reset();
}

WorldMapLayout::~WorldMapLayout()
{
    DeleteLocations();
}

void WorldMapLayout::Reset ()
{
    ResetLayoutParameters();
    DeleteLocations();
    layoutComplete = true;
}

void WorldMapLayout::ResetLayoutParameters()
{
    layoutStarted = false;
    iteration = moveNumber = numGoodMoves = 0;
    T = 0.0;
    E = -1.0;
    objective.Reset();
}

void WorldMapLayout::DeleteLocations()
{
    for ( int i = 0; i < labels.Size (); ++i ) 
        delete labels.GetData(i);
    
    for ( int il = 0; il < locations.Size (); ++il ) 
        delete locations.GetData (il);
    
    labels.Empty();
    locations.Empty();
}

void WorldMapLayout::AddLocation ( int x, int y, const char *name )
{
    int locationtype = strcmp ( name, "Gateway" ) == 0 ? 
                                WORLDMAPOBJECT_GATEWAY :
                                WORLDMAPOBJECT_LOCATION;

    WorldMapInterfaceObject *thedot = new WorldMapInterfaceObject ();
    thedot->SetTYPE ( locationtype );
    thedot->SetPosition ( x, y );
    thedot->SetBasePosition ( x, y );
    locations.PutData (thedot);

    WorldMapInterfaceLabel *thelabel = new WorldMapInterfaceLabel ( mapRectangle, thedot, name );
    thelabel->SetBasePosition( x, y );
    labels.PutData (thelabel);
    
    if (layoutStarted) 
        ResetLayoutParameters();
    
    layoutComplete = false;
}

bool WorldMapLayout::IsLayoutComplete() const
{
    return layoutComplete;
}

void WorldMapLayout::StartLayout()
{
     layoutStarted = true;
    
    // Initial Random Layout
    
    for ( int i = 0; i < labels.Size (); ++i ) {
	
	WorldMapInterfaceLabel *l = labels.GetData (i);
	UplinkAssert (l);
	l->SetRandomLabelPosition();
	objective.AddObject(l);
    }
    
    for ( int il = 0; il < locations.Size (); ++il ) {
	
	WorldMapInterfaceObject *l = locations.GetData (il);
	UplinkAssert (l);
	objective.AddObject(l);
	
	// Add in feature-point neighbourhood 
	
	MapRectangle n = l->GetExtent();
	n.x1 -= n.width;
	n.y1 -= n.height;
	n.width *= 3;
	n.height *= 3;
	
	objective.AddRect(n);
    }
    
     E = objective.GetCost();
     T = 10.0 / log(3.0);
}

void WorldMapLayout::PartialLayoutLabels()
{
    int n = labels.Size();
    
    if (layoutComplete) 
        return;
    
    if (!layoutStarted)
        StartLayout();
    
    for (int i = 0; E > 0 && i < 20; i++) {
        WorldMapInterfaceLabel *l = labels.GetData(NumberGenerator::RandomNumber(n));
	    
        int origLabelPos = l->GetLabelPosition();
	    
        objective.SubObject(l);
        l->SetRandomLabelPosition();
        objective.AddObject(l);
	    
        float deltaE = objective.GetCost() - E;

        if (deltaE < 0)
            numGoodMoves++;
        else {
            if (NumberGenerator::RandomUniformNumber() <= 1.0 - exp(-deltaE/T)) {
		    
                objective.SubObject(l);
                l->SetLabelPosition(origLabelPos);
                objective.AddObject(l);
		    
                deltaE = 0;
            }
        }
	    
        E += deltaE;
    }
    
    moveNumber += 20;
        
    if (numGoodMoves >= 5*n || moveNumber >= 10*n) {
        
        if (E <= 0 || numGoodMoves <= 0 || iteration > 20)
            layoutComplete = true;
            
        iteration++;
        moveNumber = 0;
        numGoodMoves = 0;
        T -= T / 10.0;
    }
    
}

void WorldMapLayout::FullLayoutLabels()
{
    
    int startTime = EclGetAccurateTime();
    
    while (!layoutComplete && EclGetAccurateTime() - startTime < 1500)
        PartialLayoutLabels();

}
    
LList <WorldMapInterfaceObject *> &
WorldMapLayout::GetLocations() 
{
    return locations;
}

LList <WorldMapInterfaceLabel *> &
WorldMapLayout::GetLabels() 
{
    return labels;
}
    
