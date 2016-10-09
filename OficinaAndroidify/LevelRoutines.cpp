#include "LevelRoutines.h"
#include "Block.h"

extern std::list<Block*> blocks;
extern bool RUMBLE;
extern int LEVEL;

void BuildLevel(const std::vector<int> &indexes)
{
	float width = static_cast<float>(RenderingSystem::GetResolution().x);
	float height = static_cast<float>(RenderingSystem::GetResolution().y);

	// The level is always disposed in a grid.
	// Each grid cell position is the exact
	// center of the blocks, and the grid actually
	// starts at (size/2) on both width and height,
	// so we can have the new block on the actual
	// upper corner.
	// The grid is also always centered on the
	// horizontal axis.
	// Maximum Y position should be height/2 so we
	// can have a good viewing area
	vec2 blocksize(width * 0.06f, height * 0.0520845f);
	vec2 gridStart(blocksize.x / 2.0f, blocksize.y / 2.0f);
	// Centering
	gridStart.x += blocksize.x / 4.0f;
	// Let's move the upper part of the grid down a bit
	gridStart.y += blocksize.y / 2.0f;

	int maxXblocks = int(width / blocksize.x);
	int maxYblocks = int(height / blocksize.y) / 2;

	for (int i = 0; i < indexes.size(); i++) {
		int ypos = indexes[i] / maxXblocks;
		int xpos = indexes[i] - (ypos * maxXblocks);

		if (RUMBLE && LEVEL < 9) {
			xpos = maxXblocks - xpos - 1;
			ypos = maxYblocks - ypos + 1;
		}

		// Add block to collection
		blocks.push_back(new Block(gridStart.x + (xpos * (blocksize.x)),
			gridStart.y + (ypos * (blocksize.y))));
	}
}