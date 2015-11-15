// 包装函数
module.exports = function(grunt) {
    
	// 任务配置，所有插件的配置信息
	grunt.initConfig({
	
	    // 获取package.json的信息
		pkg: grunt.file.readJSON('package.json'),
		
		// uglify插件的配置信息
		uglify: {
				options: {
					stripBanners: true,
					banner: '/*! <%= pkg.name %> <%= grunt.template.today("yyyy-mm-dd") %> */\n'//添加banner
				},
				build: {
					src: 'src/snake.js',
					dest: 'build/<%=pkg.name%>-<%=pkg.version%>.min.js'
				}					
				}
			});
			
	// 加载提供"uglify"任务的插件
	grunt.loadNpmTasks('grunt-contrib-uglify');

	// 默认任务
	grunt.registerTask('default', ['uglify']);
};