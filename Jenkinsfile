pipeline {
  agent {
    node {
      label 'a40i-build-node'
    }

  }
  stages {
    stage('build') {
      steps {
        sh './build.sh BPI-M2B-1080P'
      }
    }

  }
}