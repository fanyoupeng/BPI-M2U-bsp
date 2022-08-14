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

    stage('pack') {
      parallel {
        stage('pack-uboot') {
          steps {
            archiveArtifacts 'u-boot-sunxi/u-boot.bin'
          }
        }

        stage('pack-kernel') {
          steps {
            archiveArtifacts 'linux-sunxi/vmlinux'
          }
        }

      }
    }

  }
}