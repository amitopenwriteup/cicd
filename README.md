

---

# CI/CD Pipeline with Jenkins, Docker, and Kubernetes

This repository demonstrates a **Jenkins pipeline** that automates:

1. **Checkout** source code from GitHub
2. **Build & Push** a Docker image to a container registry
3. **Deploy** the application to a Kubernetes cluster

---

## 🔧 Pipeline Overview

The pipeline uses the following key variables:

```groovy
def dockerfile = 'dockerfile'
def registry = 'localhost:5000'
def imageName = 'myimage'
def imageTag = 'latest'
def manifestFile = 'kubernetes-manifest.yaml'
def namespace = 'default'
def IMAGE_BRANCH_TAG = "${registry}/${imageName}:${imageTag}"
```

---

## 📜 Pipeline Stages

### 1. Checkout the Repository

```groovy
git branch: 'master', url: 'https://github.com/amitopenwriteup/cicd.git'
```

* Clones the source code from the GitHub repository.

---

### 2. Build and Push Docker Image

```groovy
def dockerImage = docker.build("${registry}/${imageName}:${imageTag}", "-f ${dockerfile} .")
dockerImage.push()
```

* Builds a Docker image from the `dockerfile`.
* Tags it as `localhost:5000/myimage:latest`.
* Pushes the image to the registry.

---

### 3. Apply Kubernetes Manifest

```groovy
withCredentials([file(credentialsId: '9445dd9b-e604-40cd-a802-79f747011ecb', variable: 'KUBECONFIG')]) {
    sh """
    sed \
        -e "s|{{NAMESPACE}}|${namespace}|g" \
        -e "s|{{PULL_IMAGE}}|${IMAGE_BRANCH_TAG}|g" \
        ${manifestFile} \
    | kubectl apply -f -
    """
}
```

* Uses a stored Kubeconfig credential in Jenkins to connect to the cluster.
* Replaces placeholders in the Kubernetes manifest:

  * `{{NAMESPACE}}` → `default`
  * `{{PULL_IMAGE}}` → built Docker image
* Deploys the updated manifest with `kubectl apply`.

---

## 📂 Repository Structure

```
├── Dockerfile                # Docker build instructions
├── kubernetes-manifest.yaml  # K8s deployment template with placeholders
├── Jenkinsfile               # Jenkins pipeline
└── requirements.txt          # Python dependencies (if applicable)
```

---

## 🚀 Usage

1. **Run Jenkins job**

   * The pipeline will automatically build, push, and deploy.

2. **Verify deployment**

   ```bash
   kubectl get pods -n default
   kubectl get svc -n default
   ```

3. **Access the application** using the exposed service (LoadBalancer/NodePort/Ingress depending on manifest).

---

## ✅ Prerequisites

* Jenkins with Docker & Kubernetes plugins installed
* Docker registry (local or remote)
* Kubernetes cluster accessible from Jenkins
* Properly configured **KUBECONFIG** in Jenkins credentials

---
