pipeline {
  agent any
  stages {
    stage ('build')  {
     steps {
       script {
          sh 'mkdir -p target'
          writeFile file: 'target/amit.txt', text: 'Hi this is amit'
          writeFile file: 'target/amit.md', text: 'Hi this is md file'
          }
        }
      }
    stage ('archive') {
    steps {
      archiveArtifacts artifacts: '*.txt', excludes: '*.md', followSymlinks: false
      }
     }
   }
  }
