project('mario-game-remake', ['cpp', 'c'],
        version: '0.1.0',
  meson_version: '>= 0.40.0',
)

sources = [
	'animatedtile.cpp',
	'animation.cpp',
	'base.cpp',
	'basegame.cpp',
	'block.cpp',
	'breakblockparticle.cpp',
	'brickblock.cpp',
	'camera.cpp',
	'coin.cpp',
	'enemy.cpp',
	'firesplash.cpp',
	'flower.cpp',
	'fontmanager.cpp',
	'framerate.cpp',
	'gameobject.cpp',
	'gamestate.cpp',
	'goomba.cpp',
	'item.cpp',
	'jumper.cpp',
	'koopa.cpp',
	'main.cpp',
	'mainstate.cpp',
	'mariogame.cpp',
	'marioworld.cpp',
	'mushroom.cpp',
	'node.cpp',
	'objectgroup.cpp',
	'onewayplatform.cpp',
	'particle.cpp',
	'pickup.cpp',
	'piranhaplant.cpp',
	'platform.cpp',
	'player.cpp',
	'player.cpp',
	'playerfireball.cpp',
	'portal.cpp',
	'property.cpp',
	'questionblock.cpp',
	'shineparticle.cpp',
	'soundmanager.cpp',
	'stage.cpp',
	'statemanager.cpp',
	'staticblock.cpp',
	'statictile.cpp',
	'texturemanager.cpp',
	'tile.cpp',
	'tiledmap.cpp',
	'tileset.cpp',
	'tilesets.cpp',
	'timer.cpp',
	'utils.cpp'
]



tmxlite_include_dir = './tmxlite/include'


deps = [
	dependency('sfml-system'),
	dependency('sfml-graphics'),
	dependency('sfml-audio'),
	declare_dependency(link_args:['-ltmxlite', '-L./tmxlite'], include_directories: [tmxlite_include_dir])
]


install_directory = meson.source_root() + '/dist'

resources_directory = meson.source_root() + '/resources'

install_subdir(resources_directory, install_dir:install_directory)

executable('mario-remake', sources, dependencies: deps, install:true, install_dir: install_directory)
