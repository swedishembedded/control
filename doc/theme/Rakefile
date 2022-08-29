# -----------------------------------------------------------------------------
# Global constants
# -----------------------------------------------------------------------------
THEME        = ENV.fetch('THEME', 'tutorial')
BASE_DIR     = File.dirname(__FILE__)
SAMPLES_DIR  = 'samples'
DOCKER_IMAGE = 'asciidoctor/docker-asciidoctor'

# -----------------------------------------------------------------------------
# Methods
# -----------------------------------------------------------------------------
def inside_docker?
  File.exist?('/.dockerenv')
end

def docker_asciidoctor(program = 'asciidoctor')
  return program if inside_docker?
  docker_options = [
    '--rm',
    '--tty',
    "--mount type=bind,src=#{BASE_DIR},target=#{BASE_DIR}",
    " --workdir=#{BASE_DIR}",
    " --user=#{Process.uid}:#{Process.gid}",
    DOCKER_IMAGE,
    program
  ]
  "docker run " << docker_options.join(' ')
end

def asciidoctor_pdf
   docker_asciidoctor('asciidoctor-pdf')
end

def asciidoctor
   docker_asciidoctor
end

# -----------------------------------------------------------------------------
# Directories
# -----------------------------------------------------------------------------
directory SAMPLES_DIR

# -----------------------------------------------------------------------------
# Tasks
# -----------------------------------------------------------------------------
task :default => :test

task :test => SAMPLES_DIR do
  %w(article book manpage).each do |type|
    sh %(#{asciidoctor_pdf} ) +
      %(-a pdf-stylesdir=. ) +
      %(-a pdf-style=#{THEME}.yml ) +
      %(-a pdf-fontsdir=./fonts ) +
      %(-a iconsdir=./icons ) +
      %(-a doctype=#{type} ) +
      %(-o "samples/#{type}.pdf" ) +
      %("tests/#{type}.adoc")
  end
end
