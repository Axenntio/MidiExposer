project(
	'MidiExposer',
	'cpp',
	version : '0.1',
	default_options : [
		'warning_level=3',
		'cpp_std=c++11'
	]
)

sfml = dependency('sfml-graphics')
system = build_machine.system()
if system == 'darwin'
	# Prepare for macOS
	soundDependancies = [
		dependency('appleframeworks', modules : 'CoreMIDI'),
		dependency('appleframeworks', modules : 'CoreAudio'),
		dependency('appleframeworks', modules : 'CoreFoundation')
	]
	defines='-D__MACOSX_CORE__'
elif system == 'linux'
	# Prepare for Linux
	soundDependancies = [
		dependency('alsa'),
		dependency('threads')
	]
	defines='-D__LINUX_ALSA__'
else
	error('Unknow build system ' + system)
endif



executable(
	'MidiExposer',
	[
		'main.cpp',
		'Keyboard.cpp',
		'Key.cpp',
		'Note.cpp',
		'RtMidi.cpp'
	],
	dependencies : [
		sfml,
		soundDependancies
	],
	cpp_args : defines
)