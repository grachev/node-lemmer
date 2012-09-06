{
	'targets': [
		{
			'target_name': 'lemmatizer',
			'include_dirs': [ 'include' ],
			'cflags': [
				'-O3',
				'-Wall',
				'-fomit-frame-pointer',
				'-funroll-loops',
				'-pedantic',
				'-Wno-long-long',
				'-fPIC'
			],
			'cflags_cc': [
                '-O3',
                '-Wall',
                '-fomit-frame-pointer',
                '-funroll-loops',
                '-pedantic',
                '-Wno-long-long',
                '-fPIC'
            ],
			'cflags!': [ '-fno-exceptions' ],
			'cflags_cc!': [ '-fno-exceptions' ],
			'sources': [
				'src/automaton_basic.c',
				'src/lemmatizer.c',
				'src/paradigms.c',
				'src/prediction.c',
				'src/txml.cpp'
			],
			'conditions': [
				['OS=="mac"', {
					'xcode_settings': {
						'GCC_ENABLE_CPP_EXCEPTIONS': 'YES'
					}
				}]
			]
		}
	]
}

