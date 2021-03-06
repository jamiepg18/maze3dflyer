#ifndef __Maze3D_h__
#define __Maze3D_h__

#include <ctime> //FIXME: is this portable?

#include "Cell.h"
#include "CellCoord.h"
#include "Wall.h"
#include "Prize.h"
#include "Image.h"
#include "Picture.h"

class Maze3D {
public:
   const static int wMax = 30, hMax = 30, dMax = 30;	// This is necessary because C++ doesn't do fully dynamic
										          // multidimensional arrays. I've been spoiled by Java.
   const static int prizeMax = 30, pictureMax = 30; // max # of prizes, pictures
   int nPrizes, nPrizesLeft, nPictures;

   // The following probably belong in a Maze class.
   int w, h, d;			// width (x) of maze in cells
   // sparsity: two passageway cells cannot be closer to each other than sparsity, unless
   // they are connected as directly as possible.
   int sparsity;	// how sparse the maze must be
   // branchClustering affects tendency for the maze to branch out as much as possible from any given cell.
   // The maze will branch out as much as possible up to branchClustering branches; after that, the
   // likelihood decreases. Suggested range 1-6.
   int branchClustering;
   // Number of cells, after maze generation, that are passage.
   int numPassageCells;
   // size of one cell in world coordinates. Some code might depend on cellSize = 1.0
   const static float cellSize;
   // margin around walls that we can't collide with; should be << cellSize.
   const static float wallMargin;
   static bool checkCollisions;
   static clock_t whenEntered, whenSolved, lastSolvedTime;
   static bool hasFoundExit, newBest;
   static float exitRot; // used for rotating exit decorations
   static float exitHoleRadius;
   static float exitThickness;
   static float edgeRadius;
   static float routeRadius;
   static int seeThroughRarity; // 1 out of seeThroughRarity walls will be see-through.
   /* The maze consists of both cells and walls. Each wall may be shared by two cells. */
   Cell (*cells)[hMax][dMax];		// The maze's cells
   CellCoord ccExit, ccEntrance;
   CellCoord *solutionRoute; // record of computed solution route
   /* C++ doesn't support dynamic multidimensional arrays very well, hence the following cruft. */
   // The maze's walls
   Wall (*xWalls)[Maze3D::hMax][Maze3D::dMax];    // Walls facing along X axis   [w+1][h][d]
   Wall (*yWalls)[Maze3D::hMax+1][Maze3D::dMax]; // Walls facing along Y axis   [w][h+1][d]
   Wall (*zWalls)[Maze3D::hMax][Maze3D::dMax+1];  // Walls facing along Z axis   [w][h][d+1]
   // Pointers to the open "walls" at entrance and exit.
   Wall *exitWall, *entranceWall;
   Prize prizes[prizeMax];
   Picture pictures[pictureMax];

   bool hitLockedExit;
   bool isGenerating; // true while maze is being generated

   // Maze3D::Maze3D();
   Maze3D(int _w = 8, int _h = 8, int _d = 8, int _s = 3, int _b = 2);
   void setDims(int _w = 8, int _h = 8, int _d = 8, int _s = 3, int _b = 2);
   void randomizeDims(void);
   void incrementDims(int level);

   bool IsInside(glPoint position);
   // Return true if this wall is CLOSED or is entrance or exit.
   inline bool IsWall(Wall *wall) {
      return (wall == entranceWall || wall == exitWall || wall->state == Wall::CLOSED);
   }
   inline bool isExitLocked() { return (prizes && nPrizesLeft > 0); }

   inline int volume() { return w * h * d; }
   // estimate the number of passages that will be generated for a maze of this size and sparsity.
   int estPassages() { return int((sparsity * 3 - 1) * (volume() + 0.0) / (sparsity * sparsity * sparsity)); }

   void drawXEdge(int i, int j, int k, bool forbidden = false);
   void drawYEdge(int i, int j, int k, bool forbidden = false);
   void drawZEdge(int i, int j, int k, bool forbidden = false);
   void drawOutline(void);
   void drawForbidden(void);
   void drawQueue(void);
   void drawSolutionRoute(void);
   void drawPrizes(void);
   void drawPictures(void);
   void drawCylinder(int x1, int y1, int z1, int x2, int y2, int z2);

   int solutionRouteLen;
   void computeSolution(void);

   void addPrizeAt(CellCoord &cc);
   void addPrizes(void);
   void addPictureAt(CellCoord &cc, Wall *w, char dir, Image *image);
   void addPictures(void);

   void static inline glvc(int x, int y, int z) { glVertex3f(x * cellSize, y * cellSize, z * cellSize); }


   // queue of cells to be expanded from next
   CellCoord *queue; // only populated during maze generation
   int queueSize;

private:
   int fillInDeadEnd(int x, int y, int z, CellCoord *route);
};

extern Maze3D maze;

#endif // __Maze3D_h__
