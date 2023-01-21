<!-- <script src="https://cdnjs.cloudflare.com/ajax/libs/gsap/1.19.0/TweenMax.min.js"></script>
<script src="//s3-us-west-2.amazonaws.com/s.cdpn.io/16327/DrawSVGPlugin.js?r=12"></script>
<script src="//s3-us-west-2.amazonaws.com/s.cdpn.io/16327/MorphSVGPlugin.min.js?r=1899"></script> -->


    <svg 
            class="loader"
            xmlns="http://www.w3.org/2000/svg"
            viewBox="0 0 601 401"
          >
            <!-- from  https://codepen.io/Ranjithkumar10/pen/bwPXXp   -->
            <defs>
              <filter id="goo" color-interpolation-filters="sRGB">
                <feGaussianBlur
                  in="SourceGraphic"
                  stdDeviation="7"
                  result="blur"
                />
                <feColorMatrix
                  in="blur"
                  mode="matrix"
                  values="1 0 0 0 0  0 1 0 0 0  0 0 1 0 0  0 0 0 18 -7"
                  result="goo"
                />
                <feBlend in="SourceGraphic" in2="goo" />
              </filter>
            </defs>
            <rect
              class="bg"
              x="0"
              y="0"
              width="600"
              height="400"
              fill="transparent"
            />
            <g
              class="ellipses"
              fill="rgba(252, 119, 16, 0.2)"
              filter="url(#goo)"
            >
              <ellipse class="ellipse" cx="300" cy="200" rx="16" ry="16" />
              <ellipse class="ellipse" cx="300" cy="200" rx="16" ry="16" />
              <ellipse class="ellipse" cx="300" cy="200" rx="16" ry="16" />
              <ellipse class="ellipse" cx="300" cy="200" rx="16" ry="16" />
              <ellipse class="ellipse" cx="300" cy="200" rx="16" ry="16" />
              <ellipse class="ellipse" cx="300" cy="200" rx="16" ry="16" />
              <ellipse class="ellipse" cx="300" cy="200" rx="16" ry="16" />
              <ellipse class="ellipse" cx="300" cy="200" rx="16" ry="16" />
            </g>
          </svg>